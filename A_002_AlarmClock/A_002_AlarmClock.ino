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


char daysOfTheWeek[7][12] = {"Sonntag", "Montag", "Dienstag", "Mittwoch", "Donnerstag", "Freitag", "Samstag"};
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

void setup() {

  Serial.begin(9600);

  delay(3000); // Wait for terminal

  if (! rtc.begin()) {
    Serial.println("Can't find RTC");
    while (1);
  }
  
  if (rtc.lostPower() || syncOnFirstStart) {
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

int wake1 = 1845;
int wake2 = 1825;

void loop() {
  DateTime now = rtc.now(); // Current time
  int timeint = int(now.hour())*100 + int(now.minute());
  
  matrix.setBrightness(5);
  matrix.print(timeint);    
  matrix.drawColon(true);
  matrix.writeDisplay();
  
  if ((timeint == wake1 || timeint == wake2)) {
    if (!buzzed) {
      buzzed = true;
      display.clearDisplay();
      display.setCursor(0,0);
      display.println("Get up!");
      display.display();
      matrix.blinkRate(1);
      matrix.setBrightness(15);
      buzz(5);
    }
  } else {
    if (buzzed) {
      buzzed = false;  
      matrix.blinkRate(0);
      matrix.setBrightness(1);
    }
  }

  String time = String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println(time);
  display.display();
  if (digitalRead(button3_menu) == HIGH) {
    buzz(2);
  }
}

void buzz(int n) {
  for(int i = 0; i < n; i++)
  {
    digitalWrite(buzzer, HIGH);
    delay(500);
    digitalWrite(buzzer, LOW);
    delay(500);
  }
}
