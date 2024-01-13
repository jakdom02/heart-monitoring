#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "pico/binary_info.h"
//#include "hardware/i2c.h"
#include "max30100.h"
#include "blink.h"

#define ADDR 0x57 // I2C device address
#define INT_PIN _u(16) // INT GPIO 16

typedef enum
{
  STATE_INIT,
  STATE_IDLE,
  STATE_READ_ISR,
  STATE_READ_TEMP,
  STATE_READ_DATA,
  STATCE_Count
} Main_StateMachine_e;

static Main_StateMachine_e CurrentState = STATE_INIT;
static volatile bool data_ready = false;

struct max30100_data {
  float temperature;
  float sporead;
  float hrread;
};

void gpio_callback(uint gpio, uint32_t events){

  if (gpio == INT_PIN)
  {
      CurrentState = STATE_READ_ISR;
  }
}

void Main_BoardInit()
{
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
    if (cyw43_arch_init())
    {
        printf("Wi-Fi init failed");
        return -1;
    }
    sleep_ms(2000);
    
    bus_scan(); //scaning for i2c devices
    
    max30100_reset(); //reset max30100 
    
    max30100_init(); //initialize max30100
    uint8_t isrData;
    read_isr(&isrData);
}

int main() 
{
    uint8_t isrData;
    //printf("ISR DATA: 0x%02x \n",isrData);  //read ISR

    uint8_t fifoData[] = {0x00, 0x00, 0x00, 0x00};
    uint8_t tempInteger;
    uint8_t tempFrac;
    // Loop forever
    while (true) 
    {
      switch(CurrentState)
      {
        case STATE_INIT: 
          Main_BoardInit(); 
          CurrentState = STATE_IDLE;
          break;
        case STATE_IDLE: break;
        case STATE_READ_ISR: 
          read_isr(&isrData);
          if (isrData & MAX30100_ISR_TEMP_RDY)
          {
            CurrentState = STATE_READ_TEMP; break;
          }
          if (isrData & (MAX30100_ISR_HR_RDY | MAX30100_ISR_SPO2_RDY))
          {
            CurrentState = STATE_READ_DATA; break;
          }
          CurrentState = STATE_IDLE;
          break;
        case STATE_READ_TEMP:
          max30100_read_temperature(&tempInteger,&tempFrac);
          CurrentState = STATE_IDLE; 
          break;
        case STATE_READ_DATA:      
          //printf("0x%02x%02x \n",tempInteger,tempFrac );
          max30100_read_fifo(fifoData);
          printf("%02x%02x %02x%02x\n",fifoData[0],fifoData[1],fifoData[2],fifoData[3]);
          CurrentState = STATE_IDLE;
          break;
      }
    }
}

