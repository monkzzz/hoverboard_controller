#include <Ps3Controller.h>

void notify()
{
    //--- Digital cross/circle button events ---
    //if( Ps3.event.button_down.cross ){
        //Serial.println("X");
	  //}
	  
    //if( Ps3.event.button_down.circle ){
        //Serial.println("O");
    //}
	
    //--------------- Digital D-pad button events --------------

    //Shift
    
    if( Ps3.event.button_down.up ){
        //Serial.println("up");
	
	    if (shift<10){
        shift++;
      }
	    //Serial.println(shift); 
	  }
	
    if( Ps3.event.button_down.down ){
        //Serial.println("down");
	    if (shift>1){
        shift--;
      }
	  //Serial.println(shift); 
	  }

    //Scope

     if( Ps3.event.button_down.right ){
      //Serial.println("right");
       if (scope<3){
        scope++;
      }
      //Serial.println(scope);
     }
        
    if( Ps3.event.button_down.left ){
      //Serial.println("left");
      if (scope>1){
        scope--;
      }
      //Serial.println(scope);
    }



        

    //------------- Digital shoulder button events -------------
    //if( Ps3.event.button_down.l1 ){
        //Serial.println("shoulder");
	  //}
   
    //if( Ps3.event.button_up.l1 )
    //    Serial.println("Released the left shoulder button");
    
    if( Ps3.data.button.l1 ){
        //Serial.println("trigger pressed");
        smoke = Ps3.data.button.l1;
    }else{
        smoke = 0;
    }
    

    //-------------- Digital trigger button events -------------
    //if( Ps3.event.button_down.l2 )
    //    Serial.println("trigger");
    //if( Ps3.event.button_up.l2 )
    //    Serial.println("Released the left trigger button");
    if( Ps3.data.button.l2 ){
        //Serial.println("trigger pressed");
		  zButton = Ps3.data.button.l2;
	  }else{
		  zButton = 0;
    }
	
    //--------------- Digital stick button events --------------
    //if( Ps3.event.button_down.l3 ){
    //  Serial.println("l3");
    //}
        
    
    //---------- Digital ps button events ---------
    //if( Ps3.event.button_down.ps ){
    //  Serial.println("Playstation button");
    //}
        

    //---------------- Analog stick value events ---------------
   if( abs(Ps3.event.analog_changed.stick.lx) + abs(Ps3.event.analog_changed.stick.ly) > 2 ){
       
	     joyX=Ps3.data.analog.stick.lx;
       joyY=-Ps3.data.analog.stick.ly;
       
       joyXmapped = map(joyX,0,128,1000,2000);
       joyYmapped = map(joyY,0,128,1000,2000);

       /*
       Serial.print(" x="); Serial.print(joyX, DEC);
       Serial.print(" y="); Serial.print(joyY, DEC);

       Serial.print(" x map="); Serial.print(joyXmapped, DEC);
       Serial.print(" y map="); Serial.print(joyYmapped, DEC);
       
       Serial.println();
        */
    }

   
   //---------------------- Battery events ---------------------
    if( controller_battery != Ps3.data.status.battery ){
        controller_battery = Ps3.data.status.battery;
        /*
        Serial.print("The controller battery is ");
        if( controller_battery == ps3_status_battery_charging )      Serial.println("charging");
        else if( controller_battery == ps3_status_battery_full )     Serial.println("FULL");
        else if( controller_battery == ps3_status_battery_high )     Serial.println("HIGH");
        else if( controller_battery == ps3_status_battery_low)       Serial.println("LOW");
        else if( controller_battery == ps3_status_battery_dying )    Serial.println("DYING");
        else if( controller_battery == ps3_status_battery_shutdown ) Serial.println("SHUTDOWN");
        else Serial.println("UNDEFINED");
        */
    }

}

void onConnect(){
    Serial.println("Connected.");
}

void ps3_setup(){
    
    Ps3.attach(notify);
    Ps3.attachOnConnect(onConnect);
    Ps3.begin();

    Serial.println("setup");

    if(!Ps3.isConnected()){
		Serial.println("Ps3 not detected!");
    controller_disconnected=1;
		delay(100);
	}else{
    controller_disconnected=0;
    Serial.println("Ps3 detected!");
    delay(100);
	}
}
