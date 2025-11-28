#include <stdint.h>
#include "led.h"
#include "button.h"
#include "uart.h"

/* ---------- Simple delay ---------- */

static void delay(volatile uint32_t count)
{
    while (count--) {
        __asm volatile ("nop");
    }
}

/* ---------- main ---------- */

int main(void)
{
    led_init();
    button_init();
    uart2_init(115200);   /* Baud rate: 115200 */

    uart2_write_str("System booted.\r\n");
    uart2_write_str("Press the user button to toggle LED.\r\n");

    uint8_t prev_btn = 0;
    uint8_t led_state = 0;

    while (1) {
        uint8_t btn = button_is_pressed();

        /* Rising edge: button just pressed */
        if (btn && !prev_btn) {
            led_toggle();
            led_state ^= 1U;

            uart2_write_str("Button pressed. LED is now ");
            if (led_state) {
                uart2_write_str("ON\r\n");
            } else {
                uart2_write_str("OFF\r\n");
            }

            /* crude debounce */
            delay(50000);
        }

        prev_btn = btn;
    }
}
