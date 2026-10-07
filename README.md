# Điều khiển LED bằng nút nhấn (OneButton)

Dự án nhúng điều khiển một LED bằng một nút nhấn duy nhất, sử dụng thư viện [OneButton](https://github.com/mathertel/OneButton) để phân biệt các kiểu nhấn (single click, double click) mà không cần tự viết logic chống rung (debounce) hay đo thời gian.

## 1. Chức năng

| Thao tác trên nút | Hành vi | Hàm xử lý |
|---|---|---|
| Single click (nhấn 1 lần) | Bật/tắt LED (đảo trạng thái hiện tại) | `btnPush()` → `led.flip()` |
| Double click (nhấn đúp) | Chuyển sang chế độ nháy LED, chu kỳ 200 ms | `btnDoubleClick()` → `led.blink(200)` |

Trạng thái khởi động: LED tắt (`led.off()` trong `setup()`).

### Thay đổi so với phiên bản trước

| | Phiên bản cũ | Phiên bản hiện tại |
|---|---|---|
| Kích hoạt nháy LED | Nhấn giữ lâu (long press, > 1 s) – `attachLongPressStart(btnHold)` | Nhấn đúp – `attachDoubleClick(btnDoubleClick)` |
| ON/OFF | Single click | Single click (giữ nguyên) |

## 2. Phần cứng

> Giá trị chân cụ thể được định nghĩa trong `LED.h` hoặc build flags (`LED_PIN`, `BTN_PIN`, `LED_ACT`, `BTN_ACT`). Điền lại theo mạch thực tế của bạn.

| Tín hiệu | Macro | Mô tả |
|---|---|---|
| LED | `LED_PIN` | Chân GPIO điều khiển LED |
| Mức tích cực LED | `LED_ACT` | `HIGH` hoặc `LOW` tùy cách đấu LED |
| Nút nhấn | `BTN_PIN` | Chân GPIO đọc nút |
| Mức tích cực nút | `BTN_ACT` | Mức logic khi nút được nhấn |

Lưu ý đấu nối:

- LED phải có điện trở hạn dòng nối tiếp (thường 220 Ω – 1 kΩ tùy Vf và dòng mong muốn).
- Nút nhấn nối giữa `BTN_PIN` và GND (nếu `BTN_ACT = LOW`) kèm trở kéo lên (pull-up nội hoặc ngoài 10 kΩ). Khởi tạo `OneButton button(BTN_PIN, !BTN_ACT)` cho biết nút ở trạng thái nghỉ là mức `!BTN_ACT`.
- Nếu dùng nút cơ khí trên PCB, đặt thêm tụ 100 nF song song nút để giảm nhiễu; đặt tụ decoupling 100 nF gần chân VCC của vi điều khiển.

## 3. Cấu trúc mã nguồn

```
.
├── main.cpp      # Khởi tạo, vòng lặp chính, các hàm xử lý sự kiện nút
├── LED.h         # Lớp LED (on/off/flip/blink/loop) và định nghĩa chân
└── README.md
```

### Luồng chương trình (`main.cpp`)

1. Khai báo đối tượng `LED led(LED_PIN, LED_ACT)` và `OneButton button(BTN_PIN, !BTN_ACT)`.
2. `setup()`: tắt LED, đăng ký callback `attachClick(btnPush)` và `attachDoubleClick(btnDoubleClick)`.
3. `loop()`: gọi `led.loop()` (cập nhật trạng thái nháy không chặn) và `button.tick()` (quét nút). Không dùng `delay()`, toàn bộ chạy theo `millis()` bên trong thư viện, nên hai tác vụ chạy song song không chặn nhau.

### Lớp `LED` (`LED.h`)

Các phương thức được `main.cpp` sử dụng:

| Phương thức | Chức năng |
|---|---|
| `off()` | Tắt LED |
| `flip()` | Đảo trạng thái LED |
| `blink(ms)` | Nháy LED với chu kỳ `ms` |
| `loop()` | Phải gọi liên tục trong `loop()` để duy trì nháy |

## 4. Cài đặt & nạp chương trình

Giả định: dự án build bằng PlatformIO (có `main.cpp` thay vì `.ino`).

`platformio.ini` tham khảo:

```ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
lib_deps =
    mathertel/OneButton
build_flags =
    -DLED_PIN=2
    -DLED_ACT=HIGH
    -DBTN_PIN=0
    -DBTN_ACT=LOW
```

Nếu dùng Arduino IDE: đổi `main.cpp` thành `main.ino`, cài thư viện OneButton qua Library Manager, đặt `LED.h` cùng thư mục sketch.

## 5. Kiểm thử

| # | Thao tác | Kết quả mong đợi |
|---|---|---|
| 1 | Cấp nguồn | LED tắt |
| 2 | Single click | LED bật |
| 3 | Single click lần nữa | LED tắt |
| 4 | Double click | LED nháy chu kỳ 200 ms |
| 5 | Single click khi đang nháy | LED dừng nháy và về trạng thái ON/OFF |
| 6 | Nhấn giữ > 1 s | Không có tác dụng |

Mục 5 phụ thuộc vào cách `LED::flip()` được cài đặt trong `LED.h`. Nếu LED vẫn tiếp tục nháy, cần cho `flip()` hủy chế độ blink trước khi đảo trạng thái.

## 6. Lưu ý kỹ thuật

- **Độ trễ single click:** vì đã đăng ký double click, OneButton phải đợi hết cửa sổ chờ (mặc định khoảng 400 ms) mới xác nhận là single click. Có thể chỉnh bằng `button.setClickMs(...)`; đặt quá thấp sẽ khó bấm đúp.
- **Debounce:** do OneButton xử lý (`setDebounceMs(...)` nếu cần chỉnh).
- **Không chặn:** không dùng `delay()`, có thể mở rộng thêm tác vụ khác trong `loop()` mà không ảnh hưởng độ nhạy nút.

## 7. Hướng mở rộng

- Thêm long press để đổi tốc độ nháy hoặc tắt hẳn LED.
- Lưu trạng thái LED vào bộ nhớ không bay hơi (NVS/EEPROM) để khôi phục sau khi mất nguồn.
- Điều khiển độ sáng bằng PWM.
