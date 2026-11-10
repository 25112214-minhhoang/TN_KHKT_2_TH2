#include <Arduino.h>

#define LED1 2
#define ADC_PIN 34

void setup() {
    Serial.begin(115200);
    pinMode(LED1, OUTPUT);
}

void loop() {
    // Việc 1: LED1 nháy 500ms
    digitalWrite(LED1, HIGH);
    delay(500);
    digitalWrite(LED1, LOW);
    delay(500);

    // Việc 2: Đọc biến trở mỗi 2000ms
    int val = analogRead(ADC_PIN);
    Serial.printf(">bienTro:%d\n", val);
    delay(2000);

    // Việc 3: In Heartbeat mỗi 5000ms
    Serial.println(">heartbeat:1");
    delay(5000);
    
    // Nhan xet: Tong 1 vong loop = 500+500+2000+5000 = 8000ms (Blocking hoan toan)
}
