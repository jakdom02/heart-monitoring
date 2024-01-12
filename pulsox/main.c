#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "pico/binary_info.h"
//#include "hardware/i2c.h"
#include "max30100.h"
#include "blink.h"

#define ADDR 0x57 // I2C device address
#define INT_PIN _u(16) // INT GPIO 16

volatile bool data_ready = false;

struct max30100_data {
  float temperature;
  float sporead;
  float hrread;
};

void gpio_callback(uint gpio, uint32_t events){

   if (gpio == INT_PIN) {
        //printf(".....interupt\n");
        //printf(".....in int%d\n", gpio_get(INT_PIN)); 
        data_ready = true;
    }
}

int main() {

    const uint led_pin = CYW43_WL_GPIO_LED_PIN;

    stdio_init_all();
    // Initialize LED pin
    gpio_init(led_pin);
    gpio_set_dir(led_pin, GPIO_OUT);
    // initialize INT pin
    gpio_init(INT_PIN);
    gpio_disable_pulls(INT_PIN);
    gpio_set_input_enabled(INT_PIN,1);


    
    // Initialize i2c 
    i2c_init(i2c_default, 100 * 1000);

    // Set default SDA, SCl pins
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    // set pull ups at SDA, SCL pins
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);

    // picotool declaretions 
    bi_decl(bi_program_name("PulsoxMeter"));
    bi_decl(bi_1pin_with_name(16, "Interrupt pin 1"));
    bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C));

    // set irq callback on falling edge event
    gpio_set_irq_enabled_with_callback(INT_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);

    // wifi module check 
    if (cyw43_arch_init()){
        printf("Wi-Fi init failed");
        return -1;
    }
    sleep_ms(2000);
    //scaning for i2c devices
    //printf("main start");
    bus_scan(); 
    //reset max30100 
    max30100_reset(); 
    //initialize max30100
    max30100_init();
    //read ISR 
    max30100_read_reg(0x00);
    //max30100_read_temperature();
    //printf("%d\n", gpio_get(INT_PIN)); //geting level od INT pin
    //printf("main end"); 

    
    // Loop forever
    while (true) {
      if(data_ready)
        {
          //printf("..........in loop%d\n", gpio_get(INT_PIN)); 
          //blink(led_pin);         
          printf("%2.4f", max30100_read_temperature());
          max30100_read_fifo();
          data_ready = false;
        }
        
        //printf("blink\n");
    }
}

