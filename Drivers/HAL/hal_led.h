// HAL LED interface
#ifndef HAL_LED_H
#define HAL_LED_H

#include <stdint.h>
#include <stdbool.h>

// Led id: 0 -> LED0 (PA8), 1 -> LED1 (PD2)
void hal_led_set(int id, bool on);
void hal_led_toggle(int id);

#endif // HAL_LED_H
