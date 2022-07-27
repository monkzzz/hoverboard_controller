//cables  nunchuck  esp32
//black detect    nc
//yellow sda/data  21 (wire sda)
//white scl/clk/clock 22 (wire scl)
//red sleeve 3.3v
//black sleeve gnd

#include <NintendoExtensionCtrl.h>

// Button state
int buttonOnOff = 0;   // counter for turning something on or off

Nunchuk nchuk;

void Nunchuk_setup() {
	nchuk.begin();
  Serial.println("setup");

	if (!nchuk.connect()) {
		Serial.println("Nunchuk not detected!");
    controller_disconnected=1;
		delay(100);
	}else{
    controller_disconnected=0;
    Serial.println("Nunchuk detected!");
    delay(100);
	}

}

void Nunchuk_loop() {
	//Serial.println("----- Nunchuk Demo -----"); // Making things easier to read
	
	boolean success = nchuk.update();  // Get new data from the controller
  
  //Serial.println("loop");
  
	if (!success) {  // Ruh roh
		//Serial.println("Controller disconnected!");
    controller_disconnected=1;
		delay(100);
    nchuk.connect();
    //Serial.println("nunchuk_disconnected");
    //Serial.println(nunchuk_disconnected);
	}
	else {
    //Serial.println("Controller connected!");
    controller_disconnected=0;
    //Serial.println("nunchuk_disconnected");
    //Serial.println(nunchuk_disconnected);
    
		// Read a button (on/off, C and Z)
		zButton = nchuk.buttonZ();
    cButton = nchuk.buttonC();

		//Serial.print("Z button is ");
		if (zButton == 1) {
			//Serial.print("Z");
		}
		else if (zButton == 0) {
			//Serial.print(" ");
		}
    
    //Serial.print(" ; ");
   
   //Serial.print("C button is ");
   if (cButton == 1) {
      //Serial.print("C");
      //Serial.print(shift);

      if (shift<10){
      shift++;
      }else{
        shift=1;
      }
      delay(100);
        
      buttonOnOff = !buttonOnOff;
      //Serial.print(buttonOnOff);
    }
    else if (cButton == 0) {
      //Serial.print(" ");
    }

    //Serial.println(" ");

		// Read a joystick axis (0-255,Y)
		joyY = nchuk.joyY();
    joyYmapped = map(joyY,0,255,1000,2000);

		//Serial.println("Y ");
		//Serial.print(joyY);
    //Serial.println(joyYmapped);

   // Read a joystick axis (0-255, X)
   joyX = nchuk.joyX();
   joyXmapped = map(joyX,0,255,1000,2000);

    //Serial.println(" X ");
    //Serial.print(joyX);
    //Serial.println(joyXmapped);
    
    //Serial.println(" ");
	}
}
