//common controller definitions

int joyX;
int joyY;
uint16_t joyXmapped;
uint16_t joyYmapped;
int zButton=0;
int cButton=0;
int shift=1;
int smoke=0;
int scope=1;
int controller_disconnected=0;
int controller_battery = 0;
int controller_type = 0;

//screen buttons
const int button0 = 0;  //button for white/black background and controller selector
const int button1 = 35; //button for blacklight and controller sellector

int controller_select_state = 0;         // current state of the button
int last_controller_select_state = 0;     // previous state of the button


//choose the controller
 void choose_controller(){
  // put your main code here, to run repeatedly:
  //controller sellector button
  //button 1
  // read the pushbutton input pin:
  controller_select_state = digitalRead(button1);
  //Serial.println("button 1");
  //Serial.println(controller_select_state);
  // compare the buttonState to its previous state
  if (controller_select_state != last_controller_select_state) {
    // if the state has changed, increment the counter
    if (controller_select_state == LOW) {
      // if the current state is LOW then the button
      // went from off to on:
      if(controller_type<2){
      controller_type++;
      }else{
        controller_type=0;
      }
      
    }
    // Delay a little bit to avoid bouncing
    delay(50);
  }
  // save the current state as the last state for next time through the loop
  last_controller_select_state = controller_select_state;
 }
