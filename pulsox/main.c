#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "pico/binary_info.h"
//#include "hardware/i2c.h"
#include "i2c_ox.h"
#include "blink.h"

#define ADDR 0x57 // I2C device address
#define INT_PIN _u(16) // INT pin(27) GP27

int main() {

    const uint led_pin = CYW43_WL_GPIO_LED_PIN;

    // Initialize LED pin
    gpio_init(led_pin);
    gpio_init(INT_PIN);
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

    gpio_pull_up(INT_PIN);
    // Loop forever
    while (true) {
    // Blink LED

        blink(led_pin);
        printf("blink\n");
    }
}

