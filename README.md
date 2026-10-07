# Dự án Điều khiển LED bằng nút nhấn (OneButton)

Dự án sử dụng vi điều khiển ESP32 (cấu hình môi trường `esp32doit-devkit-v1` hoặc `esp32s`) để điều khiển một LED ngoài thông qua nút nhấn. Dự án áp dụng thư viện `OneButton` để xử lý chống dội phím (debounce) và phân biệt các thao tác nhấn khác nhau mà không sử dụng hàm `delay()` gây blocking hệ thống.

## Chức năng chính
- **Single Click (Nhấn 1 lần):** Bật (ON) hoặc Tắt (OFF) LED. Lần nhấn đầu sẽ bật, lần tiếp theo sẽ tắt.
- **Double Click (Nhấn đúp):** Chuyển LED sang chế độ nháy (Blink) liên tục với chu kỳ 200ms[cite: 3].

## Sơ đồ kết nối phần cứng (Pin Mapping)

| Thành phần | Chân ESP32 | Mức logic tích cực | Ghi chú kết nối |
| :--- | :--- | :--- | :--- |
| Nút nhấn | `GPIO 0` | `LOW` | Nút nhấn BOOT có sẵn trên board hoặc nút ngoài nối GND, cấu hình pull-up. |
| LED ngoài | `GPIO 15` | `HIGH` | Cấu hình Active HIGH: GPIO 15 -> Điện trở 1kΩ -> Anode LED -> Cathode LED -> GND. |

## Yêu cầu phần mềm
- **Framework:** Arduino
- **Môi trường phát triển:** PlatformIO (VS Code)
- **Thư viện phụ thuộc:** `mathertel/OneButton @ ^2.6.1`

## Cấu hình `platformio.ini`
Các thông số chân IO và mức logic được định nghĩa trực tiếp qua `build_flags` để dễ dàng thay đổi mà không cần can thiệp vào mã nguồn C++:

```ini
[env:esp32doit-devkit-v1]
board = esp32doit-devkit-v1
build_flags = 
	'-D BTN_PIN=0U'
	'-D BTN_ACT=LOW'
	'-D LED_PIN=15U'
	'-D LED_ACT=HIGH'
