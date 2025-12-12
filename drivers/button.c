#include "button.h"

#define GPIOC_CLOCK_EN_POS  2U
#define GPIOC_CLOCK_EN_MASK (1U << GPIOC_CLOCK_EN_POS)
#define BUTTON_PIN_POS      13U
#define BUTTON_PIN_MASK     (1U << BUTTON_PIN_POS)


void button_init(void) {
    /* GPIOC clock enable is bit 2 */
    RCC->AHB2ENR |= GPIOC_CLOCK_EN_MASK;

    /* for GPIOC set bits [27:26] to 00 for input */
    GPIOC->MODER &= ~(3U << BUTTON_PIN_POS * 2);
} 

uint32_t button_read(void) {
    /* Read input data register from pin 13 (user button) */
    uint32_t pin_state = GPIOC->IDR & BUTTON_PIN_MASK;

    /* Button is active low */
    return (pin_state == 0);
}
