//Scope control

#include <Servo.h>

int servoPin = 25;

Servo servo1;

void scope_setup() 
{
    servo1.attach(servoPin);
}

void scope_loop() {
	
    if(scope == 1) {
        servo1.write(0);
        //Serial.println("position 0");
        //delay(100);
    }
	  else if(scope == 2){
        servo1.write(120);
        //Serial.println("position 120");
        //delay(100);
    }
	  else if(scope == 3){
        servo1.write(180);
        //Serial.println("position 180");
        //delay(100);
    }
    
    
}
