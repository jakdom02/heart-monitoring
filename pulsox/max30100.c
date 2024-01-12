#include "max30100.h"

#define 1BIT_FIELD   (1)
#define 2BIT_FIELD   ((1 << 1) | (1 << 0))
#define 3BIT_FIELD   ((1 << 2) | (1 << 1) | (1 << 0))
#define 4BIT_FIELD   ((1 << 3) | (1 << 2) | (1 << 1) | (1 << 0))

#define MAX30100_I2C_ADDR       (0x57)

#define MAX30100_ISR            (0x00)
#define MAX30100_ISR_A_FULL     (1BIT_FIELD << 7)
#define MAX30100_ISR_TEMP_RDY   (1BIT_FIELD << 6)
#define MAX30100_ISR_HR_RDY     (1BIT_FIELD << 5)
#define MAX30100_ISR_SPO2_RDY   (1BIT_FIELD << 4)
#define MAX30100_ISR_PWR_RDY    (1BIT_FIELD << 0)

#define MAX30100_IER                (0x01)
#define MAX30100_IER_ENB_A_FUL      (1BIT_FIELD << 7)
#define MAX30100_IER_ENB_TEMP_RDY   (1BIT_FIELD << 6)
#define MAX30100_IER_ENB_HR_RDY     (1BIT_FIELD << 5)
#define MAX30100_IER_ENB_SPO2_RDY   (1BIT_FIELD << 4)

#define MAX30100_FIFO_WRR            (0x02)
#define MAX30100_FIFO_WRR_PTR        (4BIT_FIELD << 0)

#define MAX30100_FIFO_OVFR           (0x03)
#define MAX30100_FIFO_OVFR_COUNTER   (4BIT_FIELD << 0)

#define MAX30100_FIFO_RDR            (0x04)
#define MAX30100_FIFO_RDR_PTR        (4BIT_FIELD << 0)

#define MAX30100_FIFO_DATAR          (0x05)

#define MAX30100_MODE_CONFR          (0x06)
#define MAX30100_MODE_CONFR_SHDN     (1BIT_FIELD << 7)
#define MAX30100_MODE_CONFR_RESET    (1BIT_FIELD << 6)
#define MAX30100_MODE_CONFR_TEMP_EN  (1BIT_FIELD << 5)
#define MAX30100_MODE_CONFR_MODE     (3BIT_FIELD << 0)

#define MAX30100_SPO2_CONFR             (0x07)
#define MAX30100_SPO2_CONFR_HI_RES_EN   (1BIT_FIELD << 6)
#define MAX30100_SPO2_CONFR_SPO2_SR     (3BIT_FIELD << 2)
#define MAX30100_SPO2_CONFR_LED_PW      (2BIT_FIELD << 0)

#define MAX30100_LED_CONFR                  (0x09)
#define MAX30100_LED_CONFR_RED_PA           (4BIT_FIELD << 4)
#define MAX30100_LED_CONFR_RED_PA_OFFSET    (4)
#define MAX30100_LED_CONFR_IR_PA            (4BIT_FIELD << 0)
#define MAX30100_LED_CONFR_IR_PA_OFFSET     (0)

#define MAX30100_TINTR                      (0x16)

#define MAX30100_TFRACR                     (0x17)
#define MAX30100_TFRACR_TFRAC               (4BIT_FIELD << 0)

#define MAX30100_REV_ID                     (0xFE)
#define MAX30100_PART_ID                    (0xFF)

#define MAX30100_MODE_HR_ONLY               (1BIT_FIELD << 1)
#define MAX30100_MODE_SPO2                  (2BIT_FIELD << 0)

#define MAX30100_LED_CURRENT_CONTROL_MODE_00    (0x00)
#define MAX30100_LED_CURRENT_CONTROL_MODE_44    (0x01)
#define MAX30100_LED_CURRENT_CONTROL_MODE_76    (0x02)
#define MAX30100_LED_CURRENT_CONTROL_MODE_110   (0x03)
#define MAX30100_LED_CURRENT_CONTROL_MODE_142   (0x04)
#define MAX30100_LED_CURRENT_CONTROL_MODE_174   (0x05)
#define MAX30100_LED_CURRENT_CONTROL_MODE_208   (0x06)
#define MAX30100_LED_CURRENT_CONTROL_MODE_240   (0x07)
#define MAX30100_LED_CURRENT_CONTROL_MODE_271   (0x08)
#define MAX30100_LED_CURRENT_CONTROL_MODE_306   (0x09)
#define MAX30100_LED_CURRENT_CONTROL_MODE_338   (0x0A)
#define MAX30100_LED_CURRENT_CONTROL_MODE_370   (0x0B)
#define MAX30100_LED_CURRENT_CONTROL_MODE_402   (0x0C)
#define MAX30100_LED_CURRENT_CONTROL_MODE_436   (0x0D)
#define MAX30100_LED_CURRENT_CONTROL_MODE_468   (0x0E)
#define MAX30100_LED_CURRENT_CONTROL_MODE_500   (0x0F)

#define MAX30100_SPO2_SAMPLE_RATE_CONTROL_50    (0x00)    
#define MAX30100_SPO2_SAMPLE_RATE_CONTROL_100   (0x01)    
#define MAX30100_SPO2_SAMPLE_RATE_CONTROL_167   (0x02)    
#define MAX30100_SPO2_SAMPLE_RATE_CONTROL_200   (0x03)    
#define MAX30100_SPO2_SAMPLE_RATE_CONTROL_400   (0x04)    
#define MAX30100_SPO2_SAMPLE_RATE_CONTROL_600   (0x05)    
#define MAX30100_SPO2_SAMPLE_RATE_CONTROL_800   (0x06)    
#define MAX30100_SPO2_SAMPLE_RATE_CONTROL_1000  (0x07)   

#define MAX30100_LED_PULSE_WITDH_CONTROL_200    (0x00)
#define MAX30100_LED_PULSE_WITDH_CONTROL_400    (0x01)
#define MAX30100_LED_PULSE_WITDH_CONTROL_800    (0x02)
#define MAX30100_LED_PULSE_WITDH_CONTROL_1600   (0x03)

#define MAX30100_WRITE_REGISTER( register, registerData)  \ 
    do { \
        uint8_t msg[] = {register, registerData}; \
        i2c_write_blocking(i2c_default, MAX30100_I2C_ADDR, msg, sizeof(msg), false); \
    } while(0)

#define MAX30100_READ_REGISTER( register, dataPtr, size)  \ 
    do { \
        uint8_t msg = register; \
        i2c_write_blocking(i2c_default, MAX30100_I2C_ADDR, &msg, 1, true); \
        i2c_read_blocking(i2c_default, MAX30100_I2C_ADDR, dataPtr, size, false); \
    } while(0)

#define RESULT_IS_ERROR(result)     (res < 0)
#define RESULT_IS_SUCCESS(result)   (res >= 0)

void bus_scan()
{
    for (int addr = 0; addr < (1 << 7); ++addr) 
    {
        int result;

        uint8_t rxdata;

        result = i2c_read_blocking(i2c_default, addr, &rxdata, 1, false);

        if (ret>=0)
        {
            printf("device found: 0x%02x \n", addr);
        }
    }  
}

void max30100_reset()
{
    MAX30100_WRITE_REGISTER(MAX30100_MODE_CONF, MAX30100_MODE_CONFR_RESET);
}

void max30100_init()
{
    MAX30100_WRITE_REGISTER(MAX30100_MODE_CONFR, MAX30100_MODE_CONFR_TEMP_EN | MAX30100_MODE_SPO2);

    MAX30100_WRITE_REGISTER(MAX30100_IER,   MAX30100_IER_ENB_A_FUL |
                                            MAX30100_IER_ENB_TEMP_RDY |
                                            MAX30100_IER_ENB_HR_RDY | 
                                            MAX30100_IER_ENB_SPO2_RDY);

    MAX30100_WRITE_REGISTER(MAX30100_LED_CONFR, (MAX30100_LED_CURRENT_CONTROL_MODE_76 << MAX30100_LED_CONFR_RED_PA_OFFSET) |
                                                (MAX30100_LED_CURRENT_CONTROL_MODE_76 << MAX30100_LED_CONFR_IR_PA_OFFSET));

    MAX30100_WRITE_REGISTER(MAX30100_SPO2_CONFR, 0)
}

uint16_t max30100_read_temperature()
{  
    uint8_t integer;
    uint8_t fraction;

    MAX30100_READ_REGISTER(MAX30100_TINTR, &integer,1);
    MAX30100_READ_REGISTER(MAX30100_TFRACR, &fraction,1);
  
    return ((integer << 8) | fraction);
}

void max30100_read_fifo() 
{
    uint8_t rd_reg;
    uint8_t wr_reg;
    uint8_t data_reg;
    wr_reg = 0x02;
    rd_reg = 0x04;
    data_reg = 0x05;
    uint8_t wr_ptr;
    uint8_t rd_ptr;
    uint8_t samples_to_read;
    uint8_t buf[] = {0x00, 0x00,0x00,0x00};
    uint16_t IR;
    uint16_t RED;
    //i2c_write_timeout_us(i2c_default, MAX30100_I2C_ADDR, &reg, 1, true,50); // keep control of bus
    //i2c_read_timeout_us(i2c_default, MAX30100_I2C_ADDR, &read, 1, false,50);
    i2c_write_blocking(i2c_default, MAX30100_I2C_ADDR, &wr_reg, 1, true); 
    i2c_read_blocking(i2c_default, MAX30100_I2C_ADDR, &wr_ptr, 1, false);
    i2c_write_blocking(i2c_default, MAX30100_I2C_ADDR, &rd_reg, 1, true); 
    i2c_read_blocking(i2c_default, MAX30100_I2C_ADDR, &rd_ptr, 1, false);
    
    samples_to_read =  abs(16+(int)wr_ptr-(int)rd_ptr) % 16;
    //printf("value: %d \n",samples_to_read);
    i2c_write_blocking(i2c_default, MAX30100_I2C_ADDR, &data_reg, 1, true); 
    for (int i = 0; i < samples_to_read; i++) 
    {
        
        i2c_read_blocking(i2c_default, MAX30100_I2C_ADDR, &buf[0], 1, false);
        i2c_read_blocking(i2c_default, MAX30100_I2C_ADDR, &buf[1], 1, false);
        i2c_read_blocking(i2c_default, MAX30100_I2C_ADDR, &buf[2], 1, false); 
        i2c_read_blocking(i2c_default, MAX30100_I2C_ADDR, &buf[3], 1, false);

        //uint8_t IR = {buf[1] + buf[0]};
        //uint8_t RED = {buf[3] + buf[2]};
        //RED[0] = buf[3];
        //RED[1] = buf[2];
        //printf("IR2: 0x%04x RED2: 0x%04x\n", buf[0],buf[1]);
        printf(":%02x%02x:%02x%02x\n", buf[0],buf[1],buf[2],buf[3]);
    }
}
