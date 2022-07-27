//ttgo
//screen 135 x 240
#include <SPI.h>
#include <TFT_eSPI.h> // Hardware-specific library

#include "red_shoulder.h"
#include "red_shoulder_invert.h"
//nunchuk
#include "nunchuk_image.h"
//ps3
#include "ps3_image.h"
#include "ps3_image_invert.h"

//battery4
#include "battery4.h"

TFT_eSPI tft = TFT_eSPI();       // Invoke custom library

//Screen size
int   h_screen = 135; //y
int   w_screen = 240; //x

//Center
int   h_center = h_screen/2; //y
int   w_center = w_screen/2; //x

//last shoulder image size
int h_shoulder = 100; //y
int w_shoulder = 100; //x
int red_shoulder_image_on = 0; //if image on or off

//center of last shoulder image
int h_shoulder_2 = h_shoulder/2; //y
int w_shoulder_2 = w_shoulder/2; //x

//nunchuk image size
int h_nunchuk = 100; //y
int w_nunchuk = 35; //x
int nunchuk_image_on = 0; //if image on or off

//center of nunchuck image
int h_nunchuk_2 = h_nunchuk/2; //y
int w_nunchuk_2 = w_nunchuk/2; //x

//ps3 image size
int h_ps3 = 100; //y
int w_ps3 = 100; //x
int ps3_image_on = 0; //if image on or off

//center of ps3 image
int h_ps3_2 = h_ps3/2; //y
int w_ps3_2 = w_ps3/2; //x

//hoverboard battery;
char battery1;
char battery2;

int BAT_LOW_LVL1 = 5;
int BAT_LOW_LVL2 = 3;
int BAT_LOW_LVL3 = 1;

//colors
#define BLACK   0x0000
#define WHITE   0xFFFF

#define GREY    0x5AEB
#define RED     0xF800
#define GREEN   0x07E0
#define BLUE    0x5D9B
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0

#define lightblue 0x01E9
#define darkred 0xA041 

//screen pwm
const int pwmFreq = 5000;
const int pwmResolution = 8;
const int pwmLedChannelTFT = 0;

int backlight[5] = {10,30,60,120,220};//5 niveis backlight
byte b=1;

int i=0;

bool inv=1;
int press1=0; 
int press2=0;

void tft_setup()
{
  //screen
  //botao luz
  pinMode(button0,INPUT_PULLUP);
  //botao fundo
  pinMode(button1,INPUT_PULLUP); //N TINHA PULLUP
  
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE,TFT_BLACK);  tft.setTextSize(1);
  
  ledcSetup(pwmLedChannelTFT, pwmFreq, pwmResolution);
  ledcAttachPin(TFT_BL, pwmLedChannelTFT);
  ledcWrite(pwmLedChannelTFT, backlight[b]);

  //Serial
  //Serial.println("The device started");

  //tft text
  //tft.println("Setup done");
}

void tft_light()
{  
  //button for screen light
   if(digitalRead(button1)==0){
   if(press2==0)
   {press2=1;
   tft.fillRect(78,216,44,12,TFT_BLACK);
 
   b++;
   if(b>=5)
   b=0;

   for(int i=0;i<b+1;i++)
   tft.fillRect(78+(i*7),216,3,10,BLUE);
   ledcWrite(pwmLedChannelTFT, backlight[b]);}
   }else press2=0;

//button to choose white or black background
   if(digitalRead(button0)==0){
   if(press1==0)
   {press1=1;
   inv=!inv;
   tft.invertDisplay(inv);}   
   }else press1=0;
}

void choose_controller_screen()
{
    //Controller type
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.setTextSize(3);
    tft.setCursor (100,65); //(X,y)
    //tft.print(controller_type);
    if (controller_type==0)
    {
      tft.print("None");
    }else if (controller_type==1)
    {
      tft.print("Nunchuk");
    }else if (controller_type==2)
    {
      tft.print("PS3");
    }
    
}

void tft_format()
{ 
  tft.setTextColor(TFT_YELLOW, TFT_BLACK); // Note: the new fonts do not draw the background colour
  tft.setTextSize(5);

  //Speed
  tft.setCursor (60,50); //(X,y)
  //tft.println("Speed");
  //tft.println(joyY);
  tft.println(joyYmapped);
  //tft.println(speedValue);

  //Steer
  //tft.setCursor (60,80); //(X,y)
  //tft.println("Steer");
  //tft.println(joyX);
  //tft.println(joyXmapped);
  //tft.println(steerValue);

  //Voltage
  if(battery2 > BAT_LOW_LVL1){
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
  }else if (battery2 <= BAT_LOW_LVL1 and battery2 > BAT_LOW_LVL2)
  {
    tft.setTextColor(TFT_ORANGE, TFT_BLACK);
  }else if (battery1<3 or battery2 <= BAT_LOW_LVL3)
  {
    tft.setTextColor(TFT_RED, TFT_BLACK);
    }
    
    tft.setTextSize(3);
    tft.setCursor (0,20); //(X,y)
    
    //tft.print(battery);
    //tft.print(battery1);
    //tft.print(battery2);
    tft.print("4");
    tft.print("0");
    tft.print("v");

    //Controller type
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.setTextSize(3);
    tft.setCursor (100,110); //(X,y)
    if (controller_type==0)
    {
      tft.print("NO");
    }else if (controller_type==1)
    {
      tft.print("WII");
    }else if (controller_type==2)
    {
      tft.print("PS3");
    }

  //On Off
  if(zButton == 1)
  {
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.setTextSize(3);
    tft.setCursor (185,110); //(X,y)
    tft.println("On ");
  }else{
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.setTextSize(3);
    tft.setCursor (185,110); //(X,y)
    tft.println("Off");
  }

    //Shift
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.setTextSize(3);
    tft.setCursor (185,20); //(X,y)
    tft.println("   ");
    tft.setCursor (185,20); //(X,y)
    tft.println(shift);

    
    //Controller Battery
    
    tft.setTextSize(3);
    tft.setCursor (0,110); //(X,y)
    if(controller_battery==5){
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.println("Full ");
    }else if(controller_battery==4){
    tft.setTextColor(TFT_ORANGE, TFT_BLACK);
    tft.println("High ");
    }else if(controller_battery==3){
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.println("Low ");
    }
    
    //tft.pushImage(0,80,70,36,battery4_img); //position x, position y, size x, size y

    //Smoke
    if(smoke == 1)
  {
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.setTextSize(3);
    tft.setCursor (185,50); //(X,y)
    tft.println("On ");
  }else{
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.setTextSize(3);
    tft.setCursor (185,50); //(X,y)
    tft.println("Off");
  }
    //Scope
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.setTextSize(3);
    //tft.setCursor (0,50); //(X,y)
    //tft.println("   ");
    tft.setCursor (0,50); //(X,y)
    tft.println(scope);

}

void red_shoulder_image()
{
  red_shoulder_image_on=1;
  
//RED SHOULDER INVERT
  //tft.setCursor(5,h);
  //Serial.print("RED SHOULDER INVERT");
  //tft.fillScreen(TFT_BLACK);
  //tft.print("red shoulder invert");
  //delay(2000);
  tft.fillScreen(TFT_BLACK);

  //70, 17.5, 100, 100
  tft.pushImage(w_center-w_shoulder_2,h_center-h_shoulder_2,w_shoulder,h_shoulder,red_shoulder_invert); //position x, position y, size x, size y
  delay(2000); 
}

void nunchuk_image()
{
  nunchuk_image_on=1;
  
//Nunchuk image
  //tft.setCursor(5,h);
  //Serial.print("Nunchuck image");
  //tft.fillScreen(TFT_BLACK);
  //tft.print("Nunchuk image");
  //delay(2000);
  tft.fillScreen(TFT_BLACK);

  //, , 35, 100
  tft.pushImage(w_center-w_nunchuk_2,h_center-h_nunchuk_2,w_nunchuk,h_nunchuk,nunchuk_img); //position x, position y, size x, size y
  delay(2000); 
}

void ps3_image()
{
  ps3_image_on=1;
//ps3 image
  //tft.setCursor(5,h);
  //Serial.print("ps3 image");
  //tft.fillScreen(TFT_BLACK);
  //tft.print("ps3 image");
  //delay(2000);
  tft.fillScreen(TFT_BLACK);

  //, , ?, ?
  tft.pushImage(w_center-w_ps3_2,h_center-h_ps3_2,w_ps3,h_ps3,ps3_img); //position x, position y, size x, size y
  delay(2000); 
}
