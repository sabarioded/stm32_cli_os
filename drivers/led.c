#include "led.h"

#define RCC_AHB2ENR_GPIOAEN (1U << 0)

#define LED_PIN_POS          5U
#define LED_PIN_MASK        (1U << LED_PIN_POS)

void led_init(void) {
    /* Enable GPIOA clock */
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

    /* 
        Set PA5 to output mode 01
        Moder has 2 bits for each pin
    */
    GPIOA->MODER &= ~(0x3U << (LED_PIN_POS * 2)); // clear bits [11:10]
    GPIOA->MODER |=  (0x1U << (LED_PIN_POS * 2)); // set bits [11:10] to 01
}

void led_on(void) {
    /* set output to 1 */
    GPIOA->ODR |= LED_PIN_MASK;
}

void led_off(void) {
    /* set output to 0 */
    GPIOA->ODR &= ~LED_PIN_MASK;
}

void led_toggle(void) {
    /* toggle output bit */
    GPIOA->ODR ^= LED_PIN_MASK;
}
