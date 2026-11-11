# BÁO CÁO THỰC HÀNH BUỔI 2 (TH2)
**Môn học:** Thí nghiệm Khoa học Kỹ thuật 2 (AET2022)  
**Sinh viên thực hiện:** Phạm Minh Hoàng  
**Mã sinh viên:** 25112214  
**Thiết bị:** Heltec ESP32-S3 WiFi Kit V3  

---

## 1. HÌNH ẢNH MINH CHỨNG PHẦN CỨNG

Dưới đây là mạch thực tế được sử dụng chung cho cả Bài 1 và Bài 2 trong buổi TH2 (kế thừa từ mạch TH1):
- **Nút nhấn (Ext0 Wakeup):** Nối chân GPIO 4.
- **LED (Hiển thị nhịp Timer):** Nối chân GPIO 2.
- **Biến trở (Đọc ADC):** Nối chân GPIO 34.

![Góc chụp 1 - Mạch phần cứng TH2](media/IMG_3908.jpg)
*Hình 1: Đấu nối nút nhấn, LED và biến trở vào Heltec ESP32-S3.*

![Góc chụp 2 - Mạch phần cứng TH2](media/IMG_3909.jpg)
*Hình 2: Tổng quan các dây nối từ vi điều khiển xuống Breadboard.*

---

## 2. KẾT QUẢ QUAN SÁT VÀ KẾT LUẬN

### **Bài 1: So sánh delay(), millis() (Non-blocking) và Hardware Timer**
- **Với `delay()` (Blocking):** Các tác vụ bị nghẽn (block) chờ nhau. Lệnh delay của việc đọc ADC và in Heartbeat cộng dồn khiến tổng thời gian 1 vòng `loop()` lên tới 8 giây. LED lẽ ra nháy mỗi 500ms nhưng thực tế bị trễ (Jitter) cực kỳ lớn.
- **Với `millis()` (Non-blocking):** Nhờ việc tự kiểm tra mốc thời gian riêng (`now - lanTruocLed >= 500`), LED nháy đều đặn và ADC đọc mượt mà không chờ nhau.
- **Với Hardware Timer (`esp_timer`):** Hoạt động bằng ngắt phần cứng độc lập. Đây là cách cho độ trễ (Jitter) thấp nhất, bám nhịp cực tốt kể cả khi CPU đang bận làm việc khác trong `loop()`.

### **Bài 2: Deep Sleep & Phân biệt biến SRAM vs RTC_DATA_ATTR**
- **Quan sát trạng thái biến:** 
  - Sau mỗi lần thức dậy từ Deep Sleep, biến `demThuong` (lưu ở SRAM) bị xóa và reset về 1.
  - Biến `soLanThuc` (khai báo với `RTC_DATA_ATTR`) giữ nguyên được giá trị và cộng dồn liên tục qua các chu kỳ ngủ/thức.
- **Nguyên nhân đánh thức:** Sử dụng hàm `esp_sleep_get_wakeup_cause()`, hệ thống ghi nhận đúng nguyên nhân là do Timer định giờ (sau 10s) hoặc do ngắt ngoài Ext0 (khi nhấn nút GPIO4 xuống mức LOW).

---

## 3. TRẢ LỜI CÂU HỎI ÔN TẬP BẢO VỆ VẤN ĐÁP (SLIDE 31)

1. **Vì sao viết nối tiếp bằng delay() làm sai nhịp khi có nhiều việc trong cùng loop()?**
   - Vì `delay()` bắt CPU phải dừng hoàn toàn không làm gì cả. Thời gian của nhiều lệnh delay cộng dồn lại sẽ làm chu kỳ của vòng `loop()` bị kéo dài, các công việc phía sau bị bỏ lỡ nhịp.

2. **Jitter là gì? Vì sao esp_timer cho jitter thấp hơn millis() khi loop() bận?**
   - **Jitter** là độ lệch/độ trễ giữa thời gian thực tế xảy ra sự kiện so với thời gian lý tưởng.
   - `esp_timer` dùng ngắt phần cứng (Hardware Interrupt). Khi tới giờ, bộ định thời gian sẽ ép CPU tạm dừng việc đang làm ở `loop()` để xử lý ngắt ngay lập tức, do đó Jitter gần như bằng 0.

3. **Nêu 2 lý do kỹ thuật vì sao bài học không đo dòng tiêu thụ thật bằng Ampere:**
   - Dòng điện của ESP32 trong chế độ Deep Sleep cực kỳ nhỏ (vài micro Ampe), thiết bị đo thông thường của sinh viên không thể đo chính xác.
   - Khi cắm ampe kế nối tiếp, sự thay đổi dòng điện đột ngột từ trạng thái ngủ (vài $\mu A$) sang trạng thái hoạt động (hàng trăm $mA$) gây sụt áp, làm chip bị reset liên tục.

4. **Phân biệt Light Sleep và Deep Sleep:**
   - **Light Sleep:** CPU dừng tạm, RAM và các biến giữ nguyên. Khi thức dậy, chip chạy tiếp ngay tại dòng code vừa tạm dừng.
   - **Deep Sleep:** CPU tắt hoàn toàn, toàn bộ SRAM bị xóa (chỉ chừa lại vùng nhớ RTC). Khi thức dậy, chip khởi động lại từ hàm `setup()`.

5. **RTC_DATA_ATTR dùng để làm gì? Vì sao biến thường không giữ được giá trị qua Deep Sleep?**
   - `RTC_DATA_ATTR` là từ khóa ép biến lưu vào vùng nhớ **RTC Fast Memory** – phần bộ nhớ duy nhất không bị cắt điện khi vào Deep Sleep.
   - Biến bình thường được lưu trong SRAM, mà SRAM bị ngắt điện hoàn toàn khi ngủ sâu nên dữ liệu bốc hơi.

6. **Đọc esp_sleep_get_wakeup_cause() trả về gì khi đánh thức bằng timer và ext0?**
   - Trả về `ESP_SLEEP_WAKEUP_TIMER` khi thức dậy do đồng hồ đếm ngược hết giờ.
   - Trả về `ESP_SLEEP_WAKEUP_EXT0` khi thức dậy do người dùng nhấn nút GPIO4 (sự kiện ngắt ngoài).