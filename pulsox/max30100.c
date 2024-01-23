/**
 * @file max30100.c
 * @author T.Mroz J.Domagala
 */

#include "max30100.h"

#define MAX30100_WRITE_REGISTER( register, registerData) do { \
        uint8_t msg[] = {register, registerData}; \
        i2c_write_blocking(i2c_default, MAX30100_I2C_ADDR, msg, sizeof(msg), false); \
    } while(0)

#define MAX30100_READ_REGISTER( register, dataPtr, size) do { \
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

        if (result>=0)
        {
            printf("device found: 0x%02x \n", addr);
        }
    }  
}
void read_isr(uint8_t* dataPtr)
{
    MAX30100_READ_REGISTER(MAX30100_ISR,dataPtr,1);
}

void max30100_reset()
{
    MAX30100_WRITE_REGISTER(MAX30100_MODE_CONFR, MAX30100_MODE_CONFR_RESET);
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

    MAX30100_WRITE_REGISTER(MAX30100_SPO2_CONFR,     
                                                    (MAX30100_SPO2_SAMPLE_RATE_CONTROL_100 << MAX30100_SPO2_CONFR_SPO2_SR_OFFSET ) |
                                                     MAX30100_LED_PULSE_WITDH_CONTROL_800);
}

void max30100_read_temperature(uint8_t* integerPtr,uint8_t* fractionPtr)
{  
    //uint8_t integer;
    //uint8_t fraction;

    MAX30100_READ_REGISTER(MAX30100_TINTR, integerPtr,1);
    MAX30100_READ_REGISTER(MAX30100_TFRACR, fractionPtr,1);
  
    //return ((integer << 8) | fraction);
}

void max30100_read_fifo(uint8_t* dataPtr) 
{

    uint8_t wr_ptr_value;
    uint8_t rd_ptr_value;
    uint8_t samples_to_read;
    MAX30100_READ_REGISTER(MAX30100_FIFO_WRR,&wr_ptr_value,1);
    MAX30100_READ_REGISTER(MAX30100_FIFO_RDR,&rd_ptr_value,1);

    samples_to_read =  1; //abs(16+(int)wr_ptr_value-(int)rd_ptr_value) % 16;

    for (int i = 0; i < samples_to_read; i++) 
    {
        MAX30100_READ_REGISTER(MAX30100_FIFO_DATAR,dataPtr,4);
        ((dataPtr[0] << 8) | dataPtr[1]) >> 2;
        ((dataPtr[2] << 8) | dataPtr[3]) >> 2;
    }
}
