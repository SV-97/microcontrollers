#include <SPI.h>
#include <MFRC522.h>
#define SS_PIN 53
#define RST_PIN 5

MFRC522 mfrc522(SS_PIN, RST_PIN);

int relay = 12;
char payload[2] = {'\0', '\0'};
int filled = 0;

void setup() {
    pinMode(relay, OUTPUT);
    for (int i=1; i<=53; i++) {
        pinMode(i, OUTPUT);
    }
    Serial.begin(9600);
    SPI.begin();
    mfrc522.PCD_Init();
}

void loop() {
    if (Serial.available() > 0) {
        char new_byte = Serial.read();
        if (new_byte != 10) {
            payload[filled] = new_byte;
            filled++;
        } else {
            payload[filled] = '\0';
            filled = 0;
            Serial.print("Total Payload: ");
            Serial.println(payload);
            int pin = (payload[0] - 48) * 10 + (payload[1] - 48);
            if (pin > 0 && pin <= 53) {
                Serial.println(pin);
                digitalWrite(pin, !digitalRead(pin));
            }
        }
    }

    if ( mfrc522.PICC_IsNewCardPresent()) {
        if ( mfrc522.PICC_ReadCardSerial()) {
            bool cond = (
                mfrc522.uid.uidByte[0] == 0xC4 &&
                mfrc522.uid.uidByte[1] == 0x3F &&
                mfrc522.uid.uidByte[2] == 0x62 &&
                mfrc522.uid.uidByte[3] == 0xBB);
            if (cond) {
                digitalWrite(relay, !digitalRead(relay));
                delay(1000);
            }
        }
    }
}
 
