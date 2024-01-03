#include "i2c_ox.h"

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