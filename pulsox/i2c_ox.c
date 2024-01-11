#include "i2c_ox.h"

#define ADDR  0x57

void bus_scan(){
    for (int addr = 0; addr < (1 << 7); ++addr) {
        int ret;

        uint8_t rxdata;

        ret = i2c_read_blocking(i2c_default, addr, &rxdata, 1, false);

        if(ret>=0){
            printf("device found: 0x%02x \n", addr);
        }
    }  
}
void max30100_reset(){
    uint8_t buf[] = {0x06, 0x40}; 
    i2c_write_blocking(i2c_default, ADDR, buf, 2, false);
}

void max30100_init(){

    uint8_t buf[] = {0x06, 0x0B};
    i2c_write_blocking(i2c_default, ADDR, buf, 2, false);

    buf[0] = 0x01, buf[1] = 0xF0;
    i2c_write_blocking(i2c_default, ADDR, buf, 2, false);

    buf[0] = 0x09, buf[1] = 0x22;
    i2c_write_blocking(i2c_default, ADDR, buf, 2, false);

    buf[0] = 0x07, buf[1] = 0x00;
    i2c_write_blocking(i2c_default, ADDR, buf, 2, false);
}

void max30100_read_reg(uint8_t reg) {
    uint8_t read;
    //i2c_write_timeout_us(i2c_default, ADDR, &reg, 1, true,50); // keep control of bus
    //i2c_read_timeout_us(i2c_default, ADDR, &read, 1, false,50);
    i2c_write_blocking(i2c_default, ADDR, &reg, 1, true); 
    i2c_read_blocking(i2c_default, ADDR, &read, 1, false);
    printf("value: 0x%02x \n", read);
}

float max30100_read_temperature()
{
    
    uint8_t int_reg;
    int_reg = 0x16;
    uint8_t frac_reg;
    frac_reg = 0x17;
    uint8_t integer;
    uint8_t fraction;
    float temp;
    i2c_write_blocking(i2c_default, ADDR, &int_reg, 1, true); 
    i2c_read_blocking(i2c_default, ADDR, &integer, 1, false);
    i2c_write_blocking(i2c_default, ADDR, &frac_reg, 1, true); 
    i2c_read_blocking(i2c_default, ADDR, &fraction, 1, false);
    temp = (float)integer;
    //max30100_read_reg(0x00);
    return temp+fraction*0.0625;
    //printf("%02d \n", fraction*0.0625);
    

   //max30100_read_reg(0x16);
   //max30100_read_reg(0x17);
}
void max30100_read_fifo() {
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
    //i2c_write_timeout_us(i2c_default, ADDR, &reg, 1, true,50); // keep control of bus
    //i2c_read_timeout_us(i2c_default, ADDR, &read, 1, false,50);
    i2c_write_blocking(i2c_default, ADDR, &wr_reg, 1, true); 
    i2c_read_blocking(i2c_default, ADDR, &wr_ptr, 1, false);
    i2c_write_blocking(i2c_default, ADDR, &rd_reg, 1, true); 
    i2c_read_blocking(i2c_default, ADDR, &rd_ptr, 1, false);
    
    samples_to_read =  abs(16+(int)wr_ptr-(int)rd_ptr) % 16;
    //printf("value: %d \n",samples_to_read);
    i2c_write_blocking(i2c_default, ADDR, &data_reg, 1, true); 
    for (int i = 0; i < samples_to_read; i++) 
    {
        
        i2c_read_blocking(i2c_default, ADDR, &buf[0], 1, false);
        i2c_read_blocking(i2c_default, ADDR, &buf[1], 1, false);
        i2c_read_blocking(i2c_default, ADDR, &buf[2], 1, false); 
        i2c_read_blocking(i2c_default, ADDR, &buf[3], 1, false);

        //uint8_t IR = {buf[1] + buf[0]};
        //uint8_t RED = {buf[3] + buf[2]};
        //RED[0] = buf[3];
        //RED[1] = buf[2];
        //printf("IR2: 0x%04x RED2: 0x%04x\n", buf[0],buf[1]);
        printf(":%02x%02x:%02x%02x\n", buf[0],buf[1],buf[2],buf[3]);
    }

}