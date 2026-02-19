#include "hal_led.h"
#include "gd32f30x_gpio.h"

void hal_led_set(int id, bool on)
{
    if (id == 0) {
        gpio_bit_write(GPIOA, GPIO_PIN_8, on ? SET : RESET);
    } else if (id == 1) {
        gpio_bit_write(GPIOD, GPIO_PIN_2, on ? SET : RESET);
    }
}

void hal_led_toggle(int id)
{
    if (id == 0) {
        uint32_t octl = GPIO_OCTL(GPIOA);
        GPIO_BOP(GPIOA) = ((octl & GPIO_PIN_8) << 16u) | (~octl & GPIO_PIN_8);
    } else if (id == 1) {
        uint32_t octl = GPIO_OCTL(GPIOD);
        GPIO_BOP(GPIOD) = ((octl & GPIO_PIN_2) << 16u) | (~octl & GPIO_PIN_2);
    }
}
