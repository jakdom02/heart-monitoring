#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "pico/binary_info.h"
//#include "hardware/i2c.h"
#include "i2c_ox.h"
#include "blink.h"

#define ADDR 0x57 // I2C device address
#define INT_PIN _u(16) // INT GPIO 16

volatile bool data_ready = false;

void gpio_callback(uint gpio, uint32_t events){

   if (gpio == INT_PIN) {
        printf("interupt\n");
          max30100_read_reg(0x00);
          max30100_read_reg(0x16);
          max30100_read_reg(0x17);
        data_ready = true;
    }
}

int main() {

    const uint led_pin = CYW43_WL_GPIO_LED_PIN;

    // Initialize LED pin
    gpio_init(led_pin);
    gpio_init(INT_PIN);
    gpio_disable_pulls(INT_PIN);
    gpio_set_input_enabled(INT_PIN,1);
    //gpio_set_dir(INT_PIN, GPIO_IN);
    gpio_set_dir(led_pin, GPIO_OUT);

    //gpio_pull_up(INT_PIN); // to jest duza szansa ze nie jest potrzebne bo w czujniku jest pull up

    // Initialize chosen serial port
    stdio_init_all();

    // Initialize i2c 
    i2c_init(i2c_default, 100 * 1000);

    // Set default SDA, SCl pins
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
    bi_decl(bi_program_name("PulsoxMeter"));
    bi_decl(bi_1pin_with_name(16, "Interrupt pin 1"));
    bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C));

    // wifi module check 
    if (cyw43_arch_init()){
        printf("Wi-Fi init failed");
        return -1;
    }

    sleep_ms(2000);

    bus_scan();
    max30100_init(); 
    
    gpio_set_irq_enabled_with_callback(INT_PIN, GPIO_IRQ_LEVEL_LOW, true, &gpio_callback);

    // Loop forever
    while (true) {
      if(data_ready)
        {
          blink(led_pin);
          data_ready = false;
        }
        


        //printf("blink\n");
    }
}

