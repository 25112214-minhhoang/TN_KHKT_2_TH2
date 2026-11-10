#include <Arduino.h>

#define LED1 2

hw_timer_t *hwTimer = NULL;
volatile bool timerFlag = false;

void IRAM_ATTR onTimer() {
    timerFlag = true;
}

void setup() {
    Serial.begin(115200);
    pinMode(LED1, OUTPUT);

    // Khoi tao Hardware Timer 0, prescaler 80 -> 1 tick = 1us
    hwTimer = timerBegin(0, 80, true);
    timerAttachInterrupt(hwTimer, &onTimer, true);
    timerAlarmWrite(hwTimer, 500000, true); // 500,000 us = 500ms
    timerAlarmEnable(hwTimer);
}

void loop() {
    if (timerFlag) {
        timerFlag = false;
        digitalWrite(LED1, !digitalRead(LED1));
        uint32_t now = micros();
        Serial.printf(">hwTimer_tick:%u\n", now);
    }
}
