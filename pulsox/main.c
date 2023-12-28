#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "pico/binary_info.h"
//#include "hardware/i2c.h"
#include "i2c_ox.h"
//#include "blink_ox.h"



int main() {

    const uint led_pin = CYW43_WL_GPIO_LED_PIN;

    // Initialize LED pin
    gpio_init(led_pin);
    gpio_set_dir(led_pin, GPIO_OUT);

    // Initialize chosen serial port
    stdio_init_all();

    // Initialize i2c 
    i2c_init(i2c_default, 100 * 1000);

    // Set default SDA, SCl pins
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
    bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C));

    // wifi module check 
    if (cyw43_arch_init()){
        printf("Wi-Fi init failed");
        return -1;
    }

    sleep_ms(2000);
    bus_scan();

    // Loop forever
    while (true) {
    // Blink LED
        cyw43_arch_gpio_put(led_pin, 1);
        sleep_ms(2000);
        cyw43_arch_gpio_put(led_pin, 0);
        sleep_ms(2000);
        printf("blink\n");
    }
}

