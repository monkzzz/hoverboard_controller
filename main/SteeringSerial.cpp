//8 bytes are sent
//the first byte is / and the last is \n
//the 6 bytes in the middle are:
//speed byte1 byte2
//steer byte3 byte4, 
//sendbyte, crc

//serial (bluetooth, ftdi) what it writes is interpreted as ascii and is sent as ascii
// the gd32 hoverboard reads it as byte decimal
//example
//i write 00000
//it reads as 48

//Serial.reads () function reads the data in bytes.
//it mean if you write a command "int a=Serial.read();", then data store in "a" in bytes.


//Serial.readString() read the serial data in string.
//It mean for "String a=Serial.readString();" command "a" store string

#include "SteeringSerial.h"

//Bluetooth uart
//#define RXD2 16
//#define TXD2 17
//Se TTGO
//#define RXD2 25
#define RXD2 27
#define TXD2 26

//Debug uart
#define RXD 3
#define TXD 1

//----------------------------------------------------------------------------
// Variables
//----------------------------------------------------------------------------
int32_t speedValue = 0;
int32_t steerValue = 0;
uint8_t upperLEDMaster = 1;
uint8_t lowerLEDMaster = 1;
uint8_t mosfetOutMaster = 1;
uint8_t upperLEDSlave = 0;
uint8_t lowerLEDSlave = 0;
uint8_t mosfetOutSlave = 0;
uint8_t beepsBackwards = 0;
uint8_t activateWeakening = 0;

void SendBuffer(uint8_t buffer[], uint8_t length);
uint16_t CalcCRC(uint8_t *ptr, int count);


//----------------------------------------------------------------------------
// Initializes the steering serial
//----------------------------------------------------------------------------
void InitSteeringSerial(void)
{
  // Set up serial communication To hoverboard
  Serial2.begin(19200, SERIAL_8N1, RXD2, TXD2);
  // Set up serial communication for arduino ide
  Serial.begin(19200, SERIAL_8N1, RXD, TXD);
  
}
//uart
//start bit (0) 8 data bits (bit0...bit7) stop bit (1)

//----------------------------------------------------------------------------
// Sets the speed value
//----------------------------------------------------------------------------
void SetSpeed(uint16_t data, float factor)
{
  //qd channel 1 = 1000 a 2000
  
  int16_t tempValue = ((float)data * 2 ) - 3000.0;  // Value -1000 to 1000 
  tempValue *= factor;
  tempValue = CLAMP(tempValue, -1000, 1000);        // Avoid calculation failure
  speedValue = tempValue;
}

//----------------------------------------------------------------------------
// Sets the steering value
//----------------------------------------------------------------------------

void SetSteer(uint16_t data)
{
  //qd channel 2 = 0 a 255
  
  int16_t tempValue = ((float)data * 2 ) - 3000.0;  // Value -1000 to 1000
  tempValue = CLAMP(tempValue, -1000, 1000);        // Avoid calculation failure
  if (speedValue < 0)
  {
    steerValue *= -1;
  }
  steerValue = tempValue;
}

//----------------------------------------------------------------------------
// Sends answer to master device
//----------------------------------------------------------------------------

void SendAnswer(void)
{
  int index = 0;
  uint8_t buffer[9];
  uint8_t byte1 = 0;
  uint8_t byte2 = 0;
  uint8_t byte3 = 0;
  uint8_t byte4 = 0;
  
  uint8_t sendByte = 0;
  
  //00000000
  
  sendByte |= (activateWeakening << 7);
  //x0000000
  sendByte |= (beepsBackwards << 6);
  //0x000000
  sendByte |= (mosfetOutSlave << 5);
  //00x00000
  sendByte |= (lowerLEDSlave << 4);
  //000x0000
  sendByte |= (upperLEDSlave << 3);
  //0000x000
  sendByte |= (mosfetOutMaster << 2);
  //00000x00
  sendByte |= (lowerLEDMaster << 1);
  //000000x0
  sendByte |= (upperLEDMaster << 0);
  //0000000x

  //example upperLEDMaster lowerLEDMaster mosfetOutMaster on
  //sendbyte 00000111 = 7  
  
  uint16_t speedValue_Format = (uint16_t)(speedValue);

  //uart only accepts 8bits at a time so i need to split 16 bytes in 2 x 8bits
  //high byte
  //low byte
  
  byte1 |= (speedValue_Format >> 8) & 0xFF; 
  //shift all bits 8 places right
  
  byte2 |= speedValue_Format & 0xFF;
  //To get the low byte, clear the high byte

  //On the receive side you have to join the bytes together.
  //(uint16_t) number = (high_byte << 8) + low_byte
  int32_t speednumber = (int16_t)((byte1 << 8) | byte2); //code in the gd32 hoverboard
  
  //speed example 1000
  //byte1 = 252 = 11111100
  //byte2 = 24  = 00011000
  //final value of speed -1000
  
  //speed example 1500
  //byte1= 0 = 	00000000
  //byte2= 0 = 	00000000
  
  //speed example 2000
  //byte1= 3 	=  00000011
  //byte2= 232 	=  11101000
  //final value of speed 1000
  

  uint16_t steerValue_Format = (uint16_t)(steerValue);
  byte3 |= (steerValue_Format >> 8) & 0xFF;
  byte4 |= steerValue_Format & 0xFF;
  
  //steer example 1000
  //byte3= 252 	= 11111100
  //byte4= 24	= 00011000
  //final value of speed -1000
  
  //steer example 1500
  //byte3= 0 = 00000000
  //byte4= 0 = 00000000
  
  //steer example 2000
  //byte3= 3 =    00000011
  //byte4= 232 =  11101000
  //final value of speed 1000
  
  // Send answer
  buffer[index++] = '/';
  
  buffer[index++] = byte1;
  buffer[index++] = byte2;
  buffer[index++] = byte3;
  buffer[index++] = byte4;
  
  buffer[index++] = sendByte;

      //Serial.println("speedValue");
      //Serial.println(speedValue);
      //Serial.println("byte1");
      //Serial.println(byte1);
      //Serial.println("byte2");
      //Serial.println(byte2);
      //Serial.println("steerValue");
      //Serial.println(steerValue);
      //Serial.println("byte3");
      //Serial.println(byte3);
      //Serial.println("byte4");
      //Serial.println(byte4);
      //Serial.println("sendByte");
      //Serial.println(sendByte);

  // Calculate CRC
  uint16_t crc = CalcCRC(buffer, index);
  buffer[index++] = (crc >> 8) & 0xFF;
  //buffer[index++] = crc & 0xFF;

  //Serial.println("CRC");
  //Serial.println(crc);
      
  // Stop byte
  buffer[index++] = '\n';
  
  SendBuffer(buffer, index);

      //crc
      //when steer/speed 1500 e leds
      //33004

//sent test
// \n
//

}

//----------------------------------------------------------------------------
// Calculates CRC value
//----------------------------------------------------------------------------
uint16_t CalcCRC(uint8_t *ptr, int count)
{
  uint16_t  crc;
  uint8_t i;
  crc = 0;
  while (--count >= 0)
  {
    crc = crc ^ (uint16_t) *ptr++ << 8;
    i = 8;
    do
    {
      if (crc & 0x8000)
      {
        crc = crc << 1 ^ 0x1021;
      }
      else
      {
        crc = crc << 1;
      }
    } while(--i);
  }
  return (crc);
}

//----------------------------------------------------------------------------
// Sends buffer
//----------------------------------------------------------------------------
void SendBuffer(uint8_t buffer[], uint8_t length)
{
  uint8_t index = 0;
  
  for(; index < length; index++)
  {      
    //Serial.println("sending answer");
    //Serial.println(index);
    Serial2.write(buffer[index]);
    //Serial.println(buffer[index]);
    //delay(1000);
    
  }
}

//----------------------------------------------------------------------------
// Sends debug infos
//----------------------------------------------------------------------------
void SendDebug()
{
  Serial.println("Speed ");
  Serial.print(speedValue);
  Serial.print("Steer");
  Serial.println(steerValue);
}
