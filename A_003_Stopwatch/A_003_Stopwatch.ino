
const int buzzer = 12;
const int button1_up = 11;

void setup() {
  pinMode(button1_up, INPUT);
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, HIGH);
  delay(100);
  digitalWrite(buzzer, LOW);

}


void loop() {
  if(digitalRead(button1_up) == 1) 
  {
    while(digitalRead(button1_up) == 1)
    {};
    int delay_ = 1000;
    bool buzzed = false;
    int i = 0;
    while(i < delay_) 
    {
      if(digitalRead(button1_up) == 1) 
      {
        buzz(2);
        buzzed = true;
        i = delay_;
        continue;
      }
      i++;
      delay(1);
    }
    if(!buzzed)
    {
      buzz(1);
    }
  }
}

void buzz(int n) {
  buzz_full(n, 100, 100);
}

void buzz_full(int n, int on, int off) {
  for(int i = 0; i < n; i++)
  {
    digitalWrite(buzzer, HIGH);
    delay(on);
    digitalWrite(buzzer, LOW);
    delay(off);
  }
}
