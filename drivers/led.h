#ifndef LED_H
#define LED_H

/**
 * @brief Initialize led driver
 */
void led_init(void);

/**
 * @brief turn led on
 */
void led_on(void);

/**
 * @brief turn led off
 */
void led_off(void);

/**
 * @brief toggle the led
 */
void led_toggle(void);

#endif /* LED_H */
