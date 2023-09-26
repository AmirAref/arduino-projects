#include <SevSeg.h>
SevSeg sevseg;

int button1_pin = 12;
int button2_pin = 11;
int pump_pin = 13;
int buzzer_pin = 10;
long irrigation_min = 5;
long a_day = 86400000;
long last_irrigation = - a_day;
bool pump_on = false;

void setup() {
  // 7segment setup parameters
  byte numDigits = 1;  // number of 7segment digits
  byte digitPins = {}; // digit pins, left empty for one digit
  // digits pins in order : {a, b, c, d, e, f, g, DP}
  byte segmentPins[] = {2, 3, 4, 5, 6, 7, 8, 9};
  bool resistorsOnSegments = true;
  // the type of seven segment
  byte hardwareConfig = COMMON_ANODE;
  // setup 7segment
  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments);
  sevseg.setBrightness(90);

  // set min modes
  Serial.begin(9600);
  pinMode(button1_pin, INPUT_PULLUP);
  pinMode(button2_pin, INPUT_PULLUP);
  pinMode(pump_pin, OUTPUT);
  pinMode(buzzer_pin, OUTPUT);
  
}

void sound_buzzer(int milli_seconds = 500){
    // turn on buzzer for a second and turn off
    digitalWrite(buzzer_pin, HIGH);
    delay(milli_seconds);
    digitalWrite(buzzer_pin, LOW);
    delay(milli_seconds);
}

void loop() {
  
  long irrigation_time = irrigation_min * 1000;
  
  // check pupm state
  if(!pump_on && (millis() - last_irrigation > a_day)){
    // turn on the pump
  	sound_buzzer();
  	digitalWrite(pump_pin, HIGH);
    pump_on = true;
    last_irrigation = millis();
  }
   
  // chcek for irrigation done
  if(pump_on && millis() - last_irrigation > irrigation_time){
    // turn off the pump
    sound_buzzer();
    sound_buzzer();
  	digitalWrite(pump_pin, LOW);
    pump_on = false;
  }

  
  
  
  // read button states
  int button1_state = digitalRead(button1_pin);
  int button2_state = digitalRead(button2_pin);
  

  // irrigration_min is in range 1,9
  // decrease the time
  if(button1_state == 0 && irrigation_min >= 2){
    irrigation_min--;
  }
  // increase the time
  else if(button2_state == 0 && irrigation_min <= 8){
    irrigation_min++;
  }
  
  // sevseg display
  sevseg.setNumber(irrigation_min);
  sevseg.refreshDisplay();
  delay(100);
  
}
