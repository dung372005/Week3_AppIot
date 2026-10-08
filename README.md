# Điều khiển LED bằng nút nhấn với OneButton (ESP32)

Dự án điều khiển LED trên ESP32 bằng một nút nhấn, sử dụng thư viện [OneButton](https://github.com/mathertel/OneButton) để nhận diện các kiểu nhấn (single click, double click) mà không cần tự xử lý chống rung (debounce) và đếm thời gian.

## 1. Chức năng

| Thao tác trên nút | Hành vi của LED |
|---|---|
| Single click (nhấn 1 lần) | Đảo trạng thái LED (ON ↔ OFF) |
| Double click (nhấn đúp) | Chuyển sang chế độ nháy LED, chu kỳ 200 ms |

Khi khởi động, LED ở trạng thái tắt.

> Phiên bản trước dùng nhấn giữ (long press) để nháy LED. Phiên bản hiện tại đã thay bằng double click, và không còn xử lý nhấn giữ.

## 2. Phần cứng

- Board: ESP32 DOIT DevKit V1 (`esp32doit-devkit-v1`)
- 1 LED (kèm điện trở hạn dòng 220 Ω – 330 Ω nếu dùng LED rời)
- 1 nút nhấn (mặc định dùng nút BOOT có sẵn trên board)

### Pin mapping

| Chức năng | GPIO | Macro | Mức tích cực |
|---|---|---|---|
| Nút nhấn | 0 | `BTN_PIN` | `LOW` (`BTN_ACT`) |
| LED | 15 | `LED_PIN` | `HIGH` (`LED_ACT`) |

### Sơ đồ nối LED rời

```
GPIO15 ──[ 220Ω ]──►|── GND
                    LED
```

Nút nhấn: một chân nối GPIO0, chân còn lại nối GND (nút BOOT trên board đã nối sẵn như vậy).

### Lưu ý phần cứng

- **GPIO0 là chân strapping.** Giữ nút này ở mức thấp lúc reset/cấp nguồn sẽ đưa ESP32 vào chế độ download. Khi đang chạy bình thường thì không ảnh hưởng, nhưng đừng giữ nút khi bật nguồn.
- **GPIO15 cũng là chân strapping** (điều khiển log boot qua UART). Dùng làm output LED vẫn chạy được, nhưng tránh gắn tải kéo mức cố định lên chân này lúc khởi động.
- Nút tích cực mức thấp (`BTN_ACT = LOW`) nên cần trở kéo lên. GPIO0 trên board DevKit đã có trở kéo lên ngoài; OneButton cũng bật `INPUT_PULLUP` mặc định.

## 3. Cấu trúc dự án

```
.
├── LED.h 
├── OneButton.h
├── README.md
├── c4.cpp
└── form.ini
```

# 4. Cấu hình build (`form.ini`)

```ini
[env]
platform = espressif32
framework = arduino
monitor_speed = 115200
upload_speed = 921600
lib_deps =
	mathertel/OneButton @ ^2.6.1

[env:esp32doit-devkit-v1]
board = esp32doit-devkit-v1
build_flags =
	'-D BTN_PIN=0U'
	'-D BTN_ACT=LOW'
	'-D LED_PIN=15U'
	'-D LED_ACT=HIGH'
```

| Mục | Giá trị | Ý nghĩa |
|---|---|---|
| `platform` | `espressif32` | Nền tảng ESP32 |
| `framework` | `arduino` | Dùng Arduino core |
| `monitor_speed` | 115200 | Baudrate Serial Monitor |
| `upload_speed` | 921600 | Tốc độ nạp firmware |
| `lib_deps` | `mathertel/OneButton @ ^2.6.1` | Thư viện xử lý nút nhấn |
| `BTN_PIN` / `BTN_ACT` | `0U` / `LOW` | Chân và mức tích cực của nút |
| `LED_PIN` / `LED_ACT` | `15U` / `HIGH` | Chân và mức tích cực của LED |

Chân và mức tích cực được truyền qua `build_flags`, nên muốn đổi chân chỉ cần sửa `platformio.ini`, không phải sửa code.


