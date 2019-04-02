#define S1 22
#define B1 23
#define B2 24
#define B3 25
#define B4 26 
#define B10 27
#define B11 28
#define Q10 29
#define Q11 30
#define Q12 31
#define Q13 32
#define Q14 33
#define Q15 34

#define RESET 52


typedef struct Timer {
    int state;
    int target_time;
    unsigned long start_time;
};

void start_timer(Timer *self);
int check_timer(Timer *self);


// Output that can be "set", "reset" and "assigned"
typedef struct Output {
    int pin;
    int _assigned;
    int _set;
};
void set_output(Output *self, int condition); // set output operation
void reset_output(Output *self, int condition); // reset output operation
void assign_output(Output *self, int condition); // assign operation
int get_state_output(Output *self); // query current state
void write_output(Output *self); // Write state to pin


// Globals
Timer t1 = {0, 0, 0};
int state = 1;
int old_state = 0;

// PAA
Output q10 = {Q10, 0, 0};
Output q11 = {Q11, 0, 0};
Output q12 = {Q12, 0, 0};
Output q13 = {Q13, 0, 0};
Output q14 = {Q14, 0, 0};
Output q15 = {Q15, 0, 0};


void setup() {
    for (int i=S1; i<=B10; i++) {
        pinMode(i, INPUT);
    }
    for (int i=Q10; i<=Q15; i++) {
        pinMode(i, OUTPUT);
    }
    pinMode(RESET, INPUT);
    Serial.begin(9600);
}

int buf_pointer = 0;
char buf[10] = {'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'};
int read_state = 0;
int on_state = 0;
int reset_state = 0;

void loop() {
    // PAE
    int s1 = digitalRead(S1);
    int b1 = digitalRead(B1);
    int b2 = digitalRead(B2);
    int b3 = digitalRead(B3);
    int b4 = digitalRead(B4);
    int b10 = digitalRead(B10);
    int b11 = digitalRead(B11);
    /*
    Serial.print("Taster: ");
    Serial.println(s1);
    */
    
    if (Serial.available() > 0) {
        char new_char = Serial.read();
        if ((int) new_char == 10) {
            buf_pointer = 0;
            read_state = 0;
        } else {
            buf[buf_pointer++] = new_char;   
            read_state = 1;
        }
    }
    if (buf[0] != '\0' && read_state==0) {
        char on[] = "EIN";
        char reset[] = "RESET";
        on_state = 1;
        reset_state = 1;
        for (int i=0; i<=10; i++) {
            Serial.print(buf[i]);
            if (buf[i] == on[i]) {
                on_state = on_state;
            } else {
                if (i < 3) {
                    on_state = 0;
                }
            }
            if (buf[i] == reset[i]) {
                reset_state = reset_state;
            } else {
                if (i < 5) {
                    reset_state = 0;
                }
            }
            buf[i] = '\0';
        }
        Serial.println("");
    }
    
    if (!reset_state) { // digitalRead(RESET)
        switch (state) {
            case 1: 
                if (b1 && b2 && on_state) {state = 2;} // && s1
                break;
            case 2: 
                if (!b3) {state = 3;}
                break;
            case 3:
                if (!b2) {state = 4;}
                break;
            case 4:
                if (!b4) {state = 5;}
                break;
            case 5:
                if (b10 || b11) {state = 6;}
                break;
            case 6:
                t1.target_time = 800;
                start_timer(&t1);
                if (check_timer(&t1)) {state = 7;}
                break;
            case 7:
                t1.target_time = 2600;
                start_timer(&t1);
                if (check_timer(&t1)) {state = 8;}
                break;
            case 8:
                t1.target_time = 500;
                start_timer(&t1);
                if (check_timer(&t1)) {state = 9;}
                break;
            case 9:
                if (!b1) {state = 10;}
                break;
            case 10:
                t1.target_time = 1000;
                start_timer(&t1);
                if (check_timer(&t1)) {
                    state = 1; 
                    on_state = 0;
                }
                break;
            default:
                state = 1;
                on_state = 0;
                break;
        }
    } else {
        state = 0;
        reset_state = 0;
    }
    if (old_state != state) {
        Serial.print("Schritt: ");
        Serial.println(state);   
        old_state = state;
    }
    
    // Ausgangszuweisungen
    set_output(&q10, state==5);
    reset_output(&q10, state==10 || state==0);
    assign_output(&q11, state==3 || state==10);
    assign_output(&q12, state==2 || state==6);
    assign_output(&q13, state==4 || state==8);
    set_output(&q14, state==4);
    reset_output(&q14, state==10 || state==0);
    set_output(&q15, state==4);
    reset_output(&q15, state==10 || state==0);
    
    // PAA schreiben
    write_output(&q10);
    write_output(&q11);
    write_output(&q12);
    write_output(&q13);
    write_output(&q14);
    write_output(&q15);
}

void start_timer(struct Timer *self) {
    if (!self->state) {    
        self->start_time = millis();
        self->state = 1;
    }
}

int check_timer(struct Timer *self) {
    int temp = millis() - self->start_time > self->target_time;
    if (temp) {self->state = 0; }
    return temp;
}

int get_state(Output *self) {
    return self->_assigned | self->_set;
}

void assign_output(Output *self, int condition) {
    self->_assigned = condition;
}

void set_output(Output *self, int condition) {
    self->_set = condition || self->_set;
}

void reset_output(Output *self, int condition) {
    self->_set = !condition && self->_set;
}

void write_output(Output *self) {
    digitalWrite(self->pin, get_state(self));
}
