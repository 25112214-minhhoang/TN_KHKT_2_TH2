#include <Arduino.h>

#define LED_PIN 2

// Bien GIU GIA TRI qua Deep Sleep (Vung nho RTC)
RTC_DATA_ATTR int soLanThuc = 0;

// Bien MAT GIA TRI qua Deep Sleep (RAM thuong)
int demThuong = 0;

void setup() {
    Serial.begin(115200);
    delay(1000);
    pinMode(LED_PIN, OUTPUT);

    soLanThuc++;
    demThuong++;

    Serial.printf("========== BOOT LAN USER ==========\n");
    Serial.printf("So lan thuc dậy (RTC_DATA_ATTR): %d\n", soLanThuc);
    Serial.printf("Biến đếm thường (SRAM): %d\n", demThuong);

    // Nháy LED báo hiệu vừa tỉnh dậy
    digitalWrite(LED_PIN, HIGH);
    delay(500);
    digitalWrite(LED_PIN, LOW);

    Serial.println("Chuan bi vao Deep Sleep 10 giay...");
    Serial.flush();

    // Thuc day bang dinh gio (10 giay)
    esp_sleep_enable_timer_wakeup(10ULL * 1000000ULL);
    esp_deep_sleep_start();
}

void loop() {
    // Loop khong bao gio chay vi Deep Sleep reset chip ve setup()
}
