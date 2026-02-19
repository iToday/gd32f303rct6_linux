/*
 * LED driver header moved to Drivers/LED for clearer project layout.
 */

#ifndef _LED_H
#define _LED_H
#include "sys.h"

/* Toggle macros */
#include "../HAL/hal_gpio.h"
#include "../HAL/hal_led.h"

static inline void LED0(uint8_t x) { hal_led_set(0, x); }
static inline void LED1(uint8_t x) { hal_led_set(1, x); }

static inline void LED0_TOGGLE(void) { hal_led_toggle(0); }
static inline void LED1_TOGGLE(void) { hal_led_toggle(1); }

/* LED access now via HAL wrappers */
void LED_Init(void);

#endif
