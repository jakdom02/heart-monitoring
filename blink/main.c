#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

int main() {

    const uint led_pin = CYW43_WL_GPIO_LED_PIN;

    // Initialize LED pin
    gpio_init(led_pin);
    gpio_set_dir(led_pin, GPIO_OUT);

    // Initialize chosen serial port
    stdio_init_all();
    if (cyw43_arch_init()){
        printf("Wi-Fi init failed");
        return -1;
    }
    // Loop forever
    while (true) {

        // Blink LED
        //printf("Blinking!\r\n");
        cyw43_arch_gpio_put(led_pin, 1);
        sleep_ms(2000);
        cyw43_arch_gpio_put(led_pin, 0);
        sleep_ms(2000);
    }
}