#include <SPI.h>
#include <Wire.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>
#include <gfxfont.h>

#include <RTClib.h>
RTC_DS3231 rtc;

#include "Adafruit_LEDBackpack.h"
Adafruit_7segment matrix = Adafruit_7segment();

bool syncOnFirstStart = false; // true, falls die Zeitinformationen der RTC mit dem PC synchronisiert werden sollen.
                               // sollte standardm��ig auf false stehen

#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

const int buzzer = 12;
const int button1_up = 11;
const int button2_down = 10;
const int button3_menu = 9;
const int button4_left = 8;
const int button5_right = 7;
bool buzzed = false; // set to true if wakeup routine was initialized this alarm

char daysOfTheWeek[7][3] = {"So", "Mo", "Di", "Mi", "Do", "Fr", "Sa"};
int wake1 = -1;
int wake2 = -1;
bool days_to_wake[2][7] = {{false, true, true, true, true, true, false}, {false, true, true, true, true, true, false}};

void setup() {

  Serial.begin(9600);

  delay(3000); // Wait for terminal

  if (! rtc.begin()) 
  {
    Serial.println("Can't find RTC");
    while (1);
  }
  
  if (rtc.lostPower() || syncOnFirstStart) 
  {
    Serial.println("Die RTC war vom Strom getrennt. Die Zeit wird neu synchronisiert.");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  display.clearDisplay();
  matrix.begin(0x70);
  matrix.setBrightness(1);
  
  pinMode(button1_up, INPUT);
  pinMode(button2_down, INPUT);
  pinMode(button3_menu, INPUT);
  pinMode(button4_left, INPUT);
  pinMode(button5_right, INPUT);

  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, HIGH);
  delay(100);
  digitalWrite(buzzer, LOW);
}

void loop() {
  DateTime now = rtc.now(); // Current time
  int timeint = int(now.hour())*100 + int(now.minute());
  matrix_time(timeint, 5);
  
  if(timeint == wake1 && days_to_wake[0][now.dayOfTheWeek()] || timeint == wake2 && days_to_wake[1][now.dayOfTheWeek()]) //wake routine
  {
    if (!buzzed) 
    {
      buzzed = true;
      display.clearDisplay();
      display.setCursor(0,0);
      display.println("Get up!");
      display.display();
      matrix.blinkRate(1);
      matrix.setBrightness(15);
      buzz(10);
      delay(10000);
      matrix.blinkRate(0);
    }
  } else {
    if (buzzed) 
    {
      buzzed = false;  
      matrix.setBrightness(1);
    }
  }

  String time_ = String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());
  oled_time();
  /*if (digitalRead(button3_menu) == HIGH) 
  {
    buzz_full(2, 50, 50);
    menu();
    buzz_full(2, 200, 100);
    delay(1000);
  }*/
  if(digitalRead(button3_menu) == 1) 
  {
    while(digitalRead(button3_menu) == 1)
    {};
    int delay_ = 1000;
    bool buzzed = false;
    int i = 0;
    while(i < delay_) 
    {
      if(digitalRead(button3_menu) == 1) 
      {
        buzz_full(3, 100, 50);
        menu_day();
        buzz_full(3, 200, 100);
        delay(1000);
        buzzed = true;
        i = delay_;
        continue;
      }
      i++;
      delay(1);
    }
    if(!buzzed)
    {
      buzz_full(2, 50, 50);
      menu();
      buzz_full(2, 200, 100);
      delay(1000);
    }
  }
}

void buzz(int n) {
  buzz_full(n, 500, 500);
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

void menu() {
  int from_right = 0;
  int time_ = 600;
  bool button3_state = true;
  matrix_time(time_, 15);
  while(true) 
  {
    oled_time();
    int grow = 1;
    int grow_counter = 0;
    int delay_input = 500;
    while(digitalRead(button1_up) == HIGH) 
    {
      if ( grow_counter > 5 && grow < 20) 
      {
         grow += 5;
         grow_counter = 0;
      } else {
        grow_counter++;
      }
      time_ += grow*pow(10, from_right);
      time_ = over_underflow(time_);
      matrix_time(time_, 15);
      delay(delay_input);
    }
    grow = 1;
    while(digitalRead(button2_down) == HIGH) 
    {
      if ( grow_counter > 5 && grow < 20) 
      {
         grow += 15;
         grow_counter = 0;
      } else {
        grow_counter++;
      }
      time_ -= grow*pow(10, from_right);
      time_ = over_underflow(time_);
      matrix_time(time_, 15);
      delay(delay_input);
    }
    if(digitalRead(button4_left) == HIGH)
    {
      from_right++;
      if(from_right > 3)
      {
        from_right = 3;
      }
      delay(250);
    }
    if(digitalRead(button5_right) == HIGH)
    {
      from_right--;
      if(from_right < 0)
      {
        from_right = 0;
      }
      delay(250);
    }
    if(digitalRead(button3_menu) == HIGH)
    {
      if(!button3_state) 
      {
        wake2 = wake1;
        wake1 = time_;
        return;
      }
    } else {
      button3_state = false;
    }
  }
}

void menu_day() {
  int selected_x = 1;
  int selected_y = 0;
  while(true)
  {
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(64-35, 0);
    display.println("Alarm " + String(selected_x));
    display.drawLine(0, 21, display.width(), 21, WHITE);
    String placed = "";
    if(days_to_wake[selected_x-1][selected_y])
    {
      placed = "[x]";
    } else {
      placed = "[ ]";
    }
    display.setCursor(64-20, 30);
    display.println(String(daysOfTheWeek[selected_y]) + ":" + String(placed));
    display.display();
    
    if(digitalRead(button3_menu) == 1) 
    {
      while(digitalRead(button3_menu) == 1)
      {};
      int delay_ = 500;
      bool buzzed = false;
      int i = 0;
      while(i < delay_) 
      {
        if(digitalRead(button3_menu) == 1) 
        {
          return;
          buzzed = true;
          i = delay_;
          continue;
        }
        i++;
        delay(1);
      }
      if(!buzzed)
      {
        days_to_wake[selected_x-1][selected_y] = !days_to_wake[selected_x-1][selected_y];
        delay(250);
      }
    }

    if(digitalRead(button1_up) == HIGH)
    {
      selected_y++;
      if(selected_y > 6)
      {
        selected_y = 6;
      }
      delay(250);
    }
    if(digitalRead(button2_down) == HIGH)
    {
      selected_y--;
      if(selected_y < 0)
      {
        selected_y = 0;
      }
      delay(250);
    }
    if(digitalRead(button4_left) == HIGH)
    {
      selected_x++;
      if(selected_x > 2)
      {
        selected_x = 2;
      }
      delay(250);
    }
    if(digitalRead(button5_right) == HIGH)
    {
      selected_x--;
      if(selected_x < 1)
      {
        selected_x = 1;
      }
      delay(250);
    }
  }
}

int over_underflow(int time_) {
  int hours = time_/100;
  int minutes = time_%100;
  if (minutes >= 60) 
  {
      time_ = 100*(hours+1);
  }
  if (minutes < 0) 
  {
    time_ = 100*(hours-1);
  }
  if (hours >= 24) 
  {
    time_ = 0;
  }
  if (hours < 0) 
  {
    time_ = 2400;
  }
  return time_;
}

void matrix_time(int time_, int brightness) {
  matrix.setBrightness(brightness);
  if (time_ < 1000)
  {
    matrix.writeDigitNum(0, (time_ / 1000), false);
    matrix.writeDigitNum(1, (time_ / 100) % 10, false);
    matrix.writeDigitNum(3, (time_ / 10) % 10, false);
    matrix.writeDigitNum(4, time_ % 10, false);
  } else {
    matrix.print(time_);    
  }
  matrix.drawColon(true);
  matrix.writeDisplay();
}

void oled_time() {
  DateTime now = rtc.now();
  String time_ = String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print(String(daysOfTheWeek[now.dayOfTheWeek()]));
  display.setCursor(28,0);
  display.print(time_);
  display.drawLine(0, 21, display.width(), 21, WHITE);
  display.drawLine(26, 0, 26, 21, WHITE);
  display.setCursor(0, 50-21);
  display.println("a1:  " + String(wake1/100) + ":" + String(wake1%100));
  display.setCursor(0,50);
  display.println("a2:  " + String(wake2/100) + ":" + String(wake2%100));
  display.display();
}
