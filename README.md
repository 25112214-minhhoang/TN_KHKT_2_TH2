# BÁO CÁO THỰC HÀNH BUỔI 2 (TH2)
**Môn học:** Thí nghiệm Khoa học Kỹ thuật 2 (AET2022)  
**Sinh viên thực hiện:** Phạm Minh Hoàng  
**Mã sinh viên:** 25112214  
**Thiết bị:** Heltec ESP32-S3 WiFi Kit V3  

---

## 1. MÃ NGUỒN VÀ BÀI TẬP

### **Bài 1: So sánh delay(), millis() (Non-blocking) và Hardware Timer**
- **Sơ đồ phần cứng:** Dùng chung sơ đồ phần cứng với Bài TH1 (LED - GPIO2, Biến trở - GPIO34).
- **Mã nguồn:**
  - `bai1/src/vd1_dung_delay.cpp`: Viết nối tiếp 3 công việc bằng `delay()`.
  - `bai1/src/vd2_non_blocking.cpp`: Quản lý thời gian độc lập bằng `millis()`.
  - `bai1/src/vd3_hardware_timer.cpp`: Sử dụng ngắt Timer phần cứng `esp_timer`.
- **Kết quả quan sát & Nhận xét (Jitter / Độ trễ):**
  - **Với `delay()` (Blocking):** Các công việc bị ngẽn nối tiếp. Tổng thời gian một vòng `loop()` lên tới 8000ms. LED nháy bị chậm theo chu kỳ 8s thay vì 500ms. Độ lệch Jitter cực kỳ lớn.
  - **Với `millis()` (Non-blocking):** Các công việc tự kiểm tra mốc thời gian riêng. Chu kỳ nháy LED (500ms) và đọc ADC (2000ms) diễn ra mượt mà, đúng định kỳ. Jitter rất nhỏ.
  - **Với Hardware Timer (`esp_timer`):** Hoạt động bằng ngắt phần cứng độc lập với luồng `loop()`. Dù CPU có bận xử lý tác vụ khác, ngắt ngắt timer vẫn đáp ứng chính xác theo thời gian thực (Jitter gần như bằng 0).

---

### **Bài 2: Deep Sleep Định Giờ & Đánh Thức Bằng Nút Nhấn**
- **Sơ đồ phần cứng:** Dùng chung sơ đồ TH1 (Nút nhấn - GPIO4, LED - GPIO2).
- **Mã nguồn:**
  - `bai2/src/vd1_rtc_mem_deepsleep.cpp`: Kiểm chứng vùng nhớ `RTC_DATA_ATTR` và SRAM thường.
  - `bai2/src/vd2_wakeup_cause.cpp`: Đánh thức bằng Timer định giờ hoặc Ngắt ngoài (Ext0 - GPIO4) và kiểm tra nguyên nhân bằng `esp_sleep_get_wakeup_cause()`.
- **Kết quả quan sát & Kết luận:**
  - **Biến thường (SRAM):** Sau mỗi lần thức dậy từ Deep Sleep, vi điều khiển khởi động lại từ `setup()`, biến thường luôn bị reset về giá trị ban đầu (`demThuong = 1`).
  - **Biến `RTC_DATA_ATTR`:** Giá trị biến được giữ nguyên qua các chu kỳ Deep Sleep và tăng dồn liên tục (`soLanThuc = 1, 2, 3...`).
  - **Phân biệt Sleep Mode:** Deep Sleep dừng hoàn toàn CPU và tắt phần lớn RAM, mang lại mức tiết kiệm điện cao nhất.

---

## 2. CÂU HỎI ÔN TẬP
1. **Vì sao viết nối tiếp bằng delay() làm sai nhịp?**  
   Vì `delay()` giữ chân CPU (blocking), thời gian của các lệnh delay bị cộng dồn làm tăng tổng thời gian một vòng `loop()`.
2. **Jitter là gì? Vì sao esp_timer cho jitter thấp hơn millis()?**  
   Jitter là độ lệch giữa thời gian thực tế xảy ra sự kiện và thời gian lý tưởng. `esp_timer` dùng ngắt phần cứng trực tiếp từ bộ đếm timer nên không phụ thuộc vào tiến độ chạy của vòng `loop()`.
3. **Phân biệt Light Sleep và Deep Sleep:**  
   - *Light Sleep:* CPU tạm dừng, RAM giữ nguyên, khi thức dậy tiếp tục chạy ngay tại vị trí tạm dừng.  
   - *Deep Sleep:* CPU tắt hoàn toàn, SRAM bị xóa, khi thức dậy chip khởi động lại từ `setup()`. Biến muốn nhớ phải đặt ở `RTC_DATA_ATTR`.
