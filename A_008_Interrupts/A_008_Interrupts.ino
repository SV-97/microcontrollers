int led_1 = 12;
int button = 20;
volatile byte led_1_state = 0;

void setup() {
  pinMode(led_1, OUTPUT);
  pinMode(button, INPUT);
  attachInterrupt(1, ISR_button_pressed, RISING);
}

void loop() {
  
}

void ISR_button_pressed(){
  if (digitalRead(led_1)) {
    led_1_state = 0;
    digitalWrite(led_1, 0);
  } else {
    led_1_state = 1;
    digitalWrite(led_1, 1);
  }
  byte state = 0;
  while (1) {
    if (digitalRead(button)) {
      state = 1;  
    } else {
      state = 0;
    }
    delay(200);
    if (!(digitalRead(button) == state)) {
      break;
    }
  }
}
