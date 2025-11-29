#include "led.h"
#include <stdint.h>

#define PERIPH_BASE         0x40000000UL
#define AHB2PERIPH_BASE     (PERIPH_BASE + 0x08000000UL)
#define GPIOA_BASE          (AHB2PERIPH_BASE + 0x00UL)
#define RCC_BASE            0x40021000UL
#define RCC_AHB2ENR         (*(volatile uint32_t *)(RCC_BASE + 0x4C))
#define GPIOA_MODER         (*(volatile uint32_t *)(GPIOA_BASE + 0x0))
#define GPIOA_ODR           (*(volatile uint32_t *)(GPIOA_BASE + 0x14))

#define LED_PIN             5U

void led_init(void) {
    /* Enable GPIOA clock - GPIOAEN */
    RCC_AHB2ENR |= (1U << 0);

    /* set PA5 to output mode 01 */
    GPIOA_MODER &= ~(0x3U << (LED_PIN * 2)); // clear bits [11:10]
    GPIOA_MODER |=  (0x1U << (LED_PIN *2)); // set bits [11:10] to 01
}

void led_on(void) {
    /* set output to 1 */
    GPIOA_ODR |= (1U << LED_PIN);
}

void led_off(void) {
    /* set output to 0 */
    GPIOA_ODR &= ~(1U << LED_PIN);
}

void led_toggle(void) {
    /* toggle output bit */
    GPIOA_ODR ^= (1U << LED_PIN);
}
