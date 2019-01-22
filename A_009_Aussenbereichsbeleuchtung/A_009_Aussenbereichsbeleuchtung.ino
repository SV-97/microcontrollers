#include <IRremote.h>
#include <LiquidCrystal.h>

const int ir_sensor = 11;
const int led = 12;
const int ldr = A0;
const int motion_sensor = 10;

byte led_state = 0;
IRrecv irrecv(ir_sensor);
decode_results ir_results;
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

void setup() {
    Serial.begin(9600);
    pinMode(led, OUTPUT);
    pinMode(ldr, INPUT);
    pinMode(motion_sensor, INPUT);
    irrecv.enableIRIn();
    lcd.begin(16, 2); // 16 chars in two rows
}

void loop() { 
    //Serial.println(analogRead(ldr)); //Grundhelligkeit=955; Minhelligkeit=545
    lcd.setCursor(0, 0);
    lcd.print("Luminosity  ");
    lcd.print(analogRead(ldr));
    if (irrecv.decode(&ir_results)) {
        if (ir_results.value == 16761405) {
            if (led_state) {
                led_state = 0;
                digitalWrite(led, 0);
            } else {
                led_state = 1;
                digitalWrite(led, 1);
            }
        }
        irrecv.resume();
    }
    
    if (!(led_state) && digitalRead(motion_sensor)) {
        if (analogRead(ldr) < 800) {
            digitalWrite(led, 1);
            delay(5000);
            digitalWrite(led, 0);
        } else {
            digitalWrite(led, 0);
        }
    }
    lcd.setCursor(0, 1);
    if (led_state) {
        lcd.print("Manual  ");
    } else {
        lcd.print("Automatic");
    }
}
