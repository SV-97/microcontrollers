int led_1 = 12;
int button = 7;
int poti = A0;
int rgb_b = 5;
int rgb_r = 4;
bool led_state = 0;

void setup() {
  pinMode(led_1, OUTPUT);
  pinMode(rgb_b, OUTPUT);
  pinMode(rgb_r, OUTPUT);
  pinMode(button, INPUT);
  Serial.begin(9600);
}

void loop() {
  if (digitalRead(button)) {
    if (led_state) {
      led_state = 0;
      Serial.println("Switched off");
    } else {
      led_state = 1;
      Serial.println("Switched on");
    }
    while (digitalRead(button)){
      delay(10);
    };
  }
  char poti_val;
  if (led_state) {
    poti_val = (analogRead(poti)>>2);
    Serial.println(poti_val);
  } else {
    poti_val = 0;
  }
  analogWrite(led_1, poti_val);
  analogWrite(rgb_b, poti_val);
  analogWrite(rgb_r, poti_val);
}
