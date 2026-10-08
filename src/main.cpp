#include <Arduino.h>
#include "LED.h"
#include <OneButton.h>

LED led(LED_PIN, LED_ACT);

void btnPush();
void btnDoubleClick(); // Đổi tên hàm cho đúng ngữ nghĩa

OneButton button(BTN_PIN, !BTN_ACT);

void setup()
{
    led.off();
    // Chức năng ON/OFF bằng single click (giữ nguyên)
    button.attachClick(btnPush);
    // Thay đổi: Nhấn đúp (double click) để chuyển sang chế độ nháy LED
    button.attachDoubleClick(btnDoubleClick); 
}

void loop()
{
    led.loop();
    button.tick();
}

void btnPush()
{
    led.flip();
}

void btnDoubleClick()
{
    led.blink(200); // Nháy LED với chu kỳ 200ms
}