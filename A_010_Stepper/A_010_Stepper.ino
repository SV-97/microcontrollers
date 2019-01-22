#include <Stepper.h>
int SPU = 2048; //Schritte pro Umdrehung
int button_down = 31;
int button_up = 33;
int sense_bottom = 35;
int sense_top = 37;
Stepper Motor(SPU, 3,5,4,6);
int motor_pos = 2000; // Won't work if motor misses steps
int top_pos = 1024;
int bottom_pos = 512;

void setup() {
    Serial.begin(9600);
    pinMode(button_down, INPUT_PULLUP);
    pinMode(button_up, INPUT_PULLUP);
    pinMode(sense_bottom, INPUT_PULLUP);
    pinMode(sense_top, INPUT_PULLUP);
    Motor.setSpeed(16); //max = 17
}

void loop() {
    if (!digitalRead(button_up) && digitalRead(sense_top)) {
        step_with_boundary(1);
        /*delay(100);
        unsigned long t1 = millis();
        byte button_pressed_again = 0;
        while (1) {
            if (digitalRead(button_up)) {
                button_pressed_again = 1;
            }
            if ((millis() - t1) >= 200) {
                break;
            }
        }
        if (button_pressed_again) {
            drive_to_position(top_pos);
        } else {
            step_with_boundary(-100);   
        }
        */
    }
    if (!digitalRead(button_down) && digitalRead(sense_bottom)) {
        step_with_boundary(-1);
    }
    if (!digitalRead(button_up) && !digitalRead(sense_bottom) && !digitalRead(sense_top)) {
        drive_to_position(top_pos);
    }
    if (!digitalRead(button_down) && !digitalRead(sense_bottom) && !digitalRead(sense_top)) {
        drive_to_position(bottom_pos);
    }
}

void step_with_boundary (int step_size) {
    if ((-4096 < motor_pos && step_size < 0) || (motor_pos < 4096 && step_size > 0)) {
        step_tracked(step_size);
    }
}

void step_tracked (int step_size) {
        Motor.step(step_size);
        motor_pos += step_size;
}

void drive_to_position(int pos) {
    int sign = 0;
    if (motor_pos < pos) {
        sign = 1;
    } else if (motor_pos > pos) {
        sign = -1;
    }
    while (motor_pos != pos) {
        Motor.step(sign);
        motor_pos += sign;
    }
}
