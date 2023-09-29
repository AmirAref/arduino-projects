
/// If you are using Common Anode Display, set `ON = LOW`.
const bool ON = LOW; // HIGH value as ON state for CC display. 
const bool OFF = !ON; // Reflect it, do not use LOW here.

// LED Pin Array:       a,  b, c, d, e,  f,  g, dp
//byte LED_DISPLAY[] = {8, 9, 6, 5, 4, 2, 3, 7};
byte LED_DISPLAY[] = {2, 3, 5,6 ,7 ,8 , 9, 4};

// LED Digit Value Array: 0 to 9 for common cathode display
byte DIGIT_CC[] = { 0xFC, 0x60, 0xDA, 0xF2, 0x66, 0xB6, 0xBE, 0xE0, 0xFE, 0xF6 };

int button1_pin = 11;
int button2_pin = 12;
int pump_pin = 13;
int buzzer_pin = 10;
long irrigation_min = 5;
long a_day = 86400000;
long last_irrigation = - a_day;
bool pump_on = false;



int min_to_milisec(int minutes){
  return minutes * 60 * 1000;
}

void displayDigit(byte digit) {
  if (digit < 0 || digit > 9) { return; } // skip on invalid
  //byte value = DIGIT_CC[digit];
  byte value = ON ? DIGIT_CC[digit] : ~DIGIT_CC[digit];
  byte i = 0, mask = 0b10000000;
  while (i < 8) {
    digitalWrite(LED_DISPLAY[i++], mask & value); 
    mask >>= 1; 
  }
}

void setup() {
  // 7 segment setup  
  byte i = 0;
  while (i < 8) { pinMode(LED_DISPLAY[i++], OUTPUT); }

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
  displayDigit(irrigation_min);
  delay(100);
  
}
