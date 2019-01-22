int trigger=7;
int echo=6;
long pulse_length=0;
long distance=0;
int buzzer=5;

void setup() {
  Serial.begin (9600);
  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(buzzer, OUTPUT);
}

void loop() {
  digitalWrite(trigger, LOW);
  delay(5);
  digitalWrite(trigger, HIGH);
  delay(10);
  digitalWrite(trigger, LOW);
  pulse_length = pulseIn(echo, HIGH); // Length of pulse on echo pin in µs
  byte cropped_dist = (pulse_length>>24);
  Serial.print("Cropped distance is: ");
  Serial.println(cropped_dist);
  distance = (pulse_length/2) * 0.03432; // Distance in cm
  int relative_dist = (distance*255)/500;
  Serial.print("Scaled distance is: ");
  Serial.println(relative_dist);
  //analogWrite(buzzer, 255-cropped_dist);
  digitalWrite(buzzer, 1);
  delay(cropped_dist*10);
  digitalWrite(buzzer, 0);
  delay(10);
}
