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

void max30100_init(){

    uint8_t buf[] = {0x06, 0x0B}; //0x0A ew do temp // 0x03 SPOmode
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
    i2c_write_blocking(i2c_default, ADDR, &reg, 1, true); // keep control of bus
    i2c_read_blocking(i2c_default, ADDR, &read, 1, false);
    printf("temp: 0x%02x \n", read);
}