//Pinouts
#include "pinouts.h"

#include "SteeringSerial.h"
#include "utils.h"

//Common Controller definitions
#include "common_controller.h"

//Nunchuk
#include "Nunchuk.h"

//Ps3
#include "ps3_control.h"

//Scope control
#include "scope_control.h"

//Tft
#include "Tft.h"

//        RX      TX
//UART0   GPIO3   GPIO1  -> commonly used for usb programming
//UART1   GPIO9   GPIO10
//UART2   GPIO16  GPIO17

//Define controller
//nunchuk 1
//ps3 move 2

//Deactivate images for debug
int debugz=1;

void setup()
{
  // Initialize steering serial
  InitSteeringSerial();

  //TFT
  tft_setup();

while(digitalRead(button0) == HIGH){
  
    //Choose controller
   choose_controller();
   choose_controller_screen();
   delay(100);
}
delay(500);

   tft.fillScreen(TFT_BLACK);

if (controller_type==1){
//Nunchuck
Nunchuk_setup();

}else if(controller_type==2){

//Ps3 Setup
ps3_setup();
}

//Controller disconnected
//If undecteded it won't stary

if (controller_type==1){
  
//Nunchuk disconnect  
while(controller_disconnected==1 and debugz==0){

   if(nunchuk_image_on==0){
    nunchuk_image();
     }
  Nunchuk_setup();

}
nunchuk_image_on==0;

}else if (controller_type==2){
  
//Ps3 disconnect
while(controller_disconnected==1 and debugz==0){

   if(nunchuk_image_on==0){
    nunchuk_image();
     }
    ps3_setup();
}
nunchuk_image_on==0;
}

//Scope setup
scope_setup();

}
void loop()
{
  //TFT
  tft_light();
  
  //Scope Loop
  scope_loop();

if (controller_type==1){
//Nunchuk loop
Nunchuk_loop();
}

//Speed and Steer

  //Speed
  //uint16_t channel1 = GetRCValue(0); //1000 - 2000
  //uint16_t channel1 = joyY;
  uint16_t channel1 = joyYmapped;

  //Steer
  //uint16_t channel2 = GetRCValue(1); //1000 - 2000
  //uint16_t channel2 = joyX;
  //uint16_t channel2 = joyXmapped;
  uint16_t channel2 = 1500;

  //uint16_t channel3 = GetRCValue(2); //1000 - 2000
  //uint16_t channel3 = 1500;
  //uint16_t channel3 = 1000;
  
  // Activate/deactivate speed mode
  // Change speed accelaration and maximum speed based on factor
  //float factor = channel3 < 1500 ? 0.5 : 1;
  //if channel3 <1500, factor=0.5 else =1
  //if channel3 <1500, factor=0.5 max speed 500
  //if channel3 >=1500, factor=1 max speed 1000
  float factor = 0.1*shift;
  //factor associated to a shift
  
  // Handle receiver values
  SetSpeed(channel1, factor);
  SetSteer(channel2);

  //SendDebug(); 
  
  /*
  Serial.println("channel 1 2 3");
  
  Serial.print(channel1);
  Serial.print(",");
  Serial.print(channel2);
  Serial.print(",");
  Serial.println(channel3);
  */
  
  delay(100);
  
  // Reply only when you receive data
  if (Serial2.available() > 0 or debugz==1)
  { 
    //Serial.println("serial available");
    //delay(100);

    //char character = Serial2.read();
    //battery = Serial2.read();
    
    //// store received bytes into an array /////
      char incomingByte[2] ;       //array of data
      for (int i=0; i<2; i ++){
      incomingByte[i] = Serial2.read();
        }
    
    //Serial.println("serial read");
    //Serial.println("char");
    //Serial.println(character);
    //Serial.println("battery");
    //Serial.println(battery);
    //Serial.println("array 0");
    //Serial.println(incomingByte[0]);
    //Serial.println("array 1");
    //Serial.println(incomingByte[1]);
    //delay(100);

     battery1 = incomingByte[0];
     battery2 = incomingByte[1];

//Show values on Screen  
//----------------------------------------------------------------------------
// Tft Values
//----------------------------------------------------------------------------

//make sure that the image goes blank when receives the signal from gd32
if (red_shoulder_image_on==1){
tft.fillScreen(TFT_BLACK);
red_shoulder_image_on=0;
}

tft_format();

    if (zButton == 1 and cButton == 0)
    {    
      SendAnswer();
      //Serial.println("answer sent");
      tft.setTextColor(TFT_GREEN, TFT_BLACK);
      tft.setTextSize(1);
      tft.setCursor (0,0); //(X,y)
      tft.println("Answer   ");
    }
    else
    {
      tft.setTextColor(TFT_RED, TFT_BLACK);
      tft.setTextSize(1);
      tft.setCursor (0,0); //(X,y)
      tft.println("No Answer");
    }

  }
  else
  {
    //shows image if no signal comes from the gd32
    if (red_shoulder_image_on==0 and debugz==0){
    red_shoulder_image();
    }
    
  }

//SendAnswer();
   
}
