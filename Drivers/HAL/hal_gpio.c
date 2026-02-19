#include "hal_gpio.h"
#include "gd32f30x_gpio.h"

// Implementations using FWLIB gpio helpers
void gpio_csx_set(uint8_t val)
{
    gpio_bit_write(GPIOB, GPIO_PIN_12, val ? SET : RESET);
}

void gpio_scl_set(uint8_t val)
{
    gpio_bit_write(GPIOB, GPIO_PIN_13, val ? SET : RESET);
}

void gpio_sda_set(uint8_t val)
{
    gpio_bit_write(GPIOB, GPIO_PIN_15, val ? SET : RESET);
}

uint8_t gpio_sda_get(void)
{
    return (uint8_t)gpio_input_bit_get(GPIOB, GPIO_PIN_14);
}

void gpio_togglepin(uint32_t gpio_periph, uint32_t pin)
{
    uint32_t octl;

    octl = GPIO_OCTL(gpio_periph);

    GPIO_BOP(gpio_periph) = ((octl & pin) << 16u) | (~octl & pin);
}
