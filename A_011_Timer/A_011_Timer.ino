#include <Stepper.h>
#include <TimerOne.h>  

int SPU = 2048; //Schritte pro Umdrehung
int button_down = 31;
int button_up = 33;
int sense_bottom = 35;
int sense_top = 37;
Stepper Motor(SPU, 3,5,4,6);
int motor_pos = 2000; // Won't work if motor misses steps
int top_pos = 1024;
int bottom_pos = 512;

volatile int state = 0;

void setup() {
    Serial.begin(9600);
    pinMode(button_down, INPUT_PULLUP);
    pinMode(button_up, INPUT_PULLUP);
    pinMode(sense_bottom, INPUT_PULLUP);
    pinMode(sense_top, INPUT_PULLUP);
    Motor.setSpeed(16); //max = 17
    Timer1.initialize(5*1000000);
    Timer1.attachInterrupt(ISR_Timer1);
}

void loop() {
    if (!digitalRead(button_up) && digitalRead(sense_top)) {
        Timer1.restart();
        delay(10);
        state = 1;
    }
    if (!digitalRead(button_down) && digitalRead(sense_bottom)) {
        Timer1.restart();
        delay(10);
        state = -1;
    }
    if (state == 1) {
        step_with_boundary(1);
    }
    if (state == -1) {
        step_with_boundary(-1);
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

void ISR_Timer1() {
    state = 0;
}
