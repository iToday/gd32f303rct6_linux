// Simple GPIO HAL wrappers for drivers
#ifndef HAL_GPIO_H
#define HAL_GPIO_H

#include <stdint.h>

// Chip-specific implementations use FWLIB gpio functions
void gpio_csx_set(uint8_t val);
void gpio_scl_set(uint8_t val);
void gpio_sda_set(uint8_t val);
uint8_t gpio_sda_get(void);
// Toggle a GPIO pin (atomic write using BOP/OCTL)
void gpio_togglepin(uint32_t gpio_periph, uint32_t pin);

#endif // HAL_GPIO_H
