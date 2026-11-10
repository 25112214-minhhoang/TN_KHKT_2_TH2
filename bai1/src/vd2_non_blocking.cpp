#include <Arduino.h>

#define LED1 2
#define ADC_PIN 34

unsigned long lanTruocLed = 0;
unsigned long lanTruocAdc = 0;
unsigned long lanTruocHeart = 0;
bool ledState = LOW;

void setup() {
    Serial.begin(115200);
    pinMode(LED1, OUTPUT);
}

void loop() {
    unsigned long now = millis();

    // Việc 1: LED1 nháy mỗi 500ms (non-blocking)
    if (now - lanTruocLed >= 500) {
        lanTruocLed = now;
        ledState = !ledState;
        digitalWrite(LED1, ledState);
    }

    // Việc 2: Đọc biến trở mỗi 2000ms
    if (now - lanTruocAdc >= 2000) {
        lanTruocAdc = now;
        int val = analogRead(ADC_PIN);
        Serial.printf(">bienTro:%d\n", val);
    }

    // Việc 3: In Heartbeat mỗi 5000ms
    if (now - lanTruocHeart >= 5000) {
        lanTruocHeart = now;
        Serial.println(">heartbeat:1");
    }
}
