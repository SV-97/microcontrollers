int switch_ = 52; 
int led = 53;

void setup() {
  pinMode(led, OUTPUT);      // sets the digital pin 13 as output
  pinMode(switch_, INPUT);   
}

previous = LOW;

void loop() {
  if (digitalRead(switch_)==HIGH && previous == LOW) {
    state = digitalRead(led);
    digitalWrite(led, !state);
  };
 previous = digitalRead(switch_); 
}