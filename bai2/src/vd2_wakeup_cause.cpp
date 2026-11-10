#include <Arduino.h>

#define BUTTON_PIN GPIO_NUM_4

void inNguyenNhanThucDay() {
    esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();
    switch (wakeup_reason) {
        case ESP_SLEEP_WAKEUP_TIMER:
            Serial.println("Thức dậy do: TIMER định giờ");
            break;
        case ESP_SLEEP_WAKEUP_EXT0:
            Serial.println("Thức dậy do: Nút bấm ngoại vi (EXT0 - GPIO4)");
            break;
        default:
            Serial.printf("Thức dậy do nguyên nhân khác / Khởi động nguồn (Code: %d)\n", wakeup_reason);
            break;
    }
}

void setup() {
    Serial.begin(115200);
    delay(1000);

    inNguyenNhanThucDay();

    // Thức dậy sau 10s HOẶC nhấn nút GPIO4 xuống mức LOW
    esp_sleep_enable_timer_wakeup(10ULL * 1000000ULL);
    esp_sleep_enable_ext0_wakeup(BUTTON_PIN, 0); // 0 = LOW

    Serial.println("Vào Deep Sleep (Đánh thức bằng 10s Timer hoặc bấm nút GPIO4)...");
    Serial.flush();
    esp_deep_sleep_start();
}

void loop() {
}
