#include "blink.h"

void blink(const uint led_pin)
{
        cyw43_arch_gpio_put(led_pin, 1);
        sleep_ms(2000);
        cyw43_arch_gpio_put(led_pin, 0);
        sleep_ms(2000);
}
