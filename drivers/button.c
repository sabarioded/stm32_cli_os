#include "button.h"
#include <stdint.h>

#define PERIPH_BASE     0x40000000UL
#define AHB2_BASE       (PERIPH_BASE + 0x8000000UL)
#define GPIOC_BASE      (AHB2_BASE + 0x800UL)
#define GPIOC_MODER     (*(volatile uint32_t *)(GPIOC_BASE + 0x00))
#define GPIOC_IDR       (*(volatile uint32_t *)(GPIOC_BASE + 0x10))

#define RCC             0x40021000UL
#define RCC_AHB2ENR     (*(volatile uint32_t *)(RCC + 0x4C))

#define BUTTON_PIN     13U

void button_init(void) {
    /* GPIOC clock enable is bit 2 */
    RCC_AHB2ENR |= (1U << 2);

    /* for GPIOC set bits [27:26] to 00 for input */
    GPIOC_MODER &= ~(3U << BUTTON_PIN * 2);
} 

uint32_t button_read(void) {
    /* Read input data register and mask for pin 13 (user button) */
    uint32_t pin_state = GPIOC_IDR & (1U << BUTTON_PIN);

    /* Button is active low */
    return (pin_state == 0);
}
