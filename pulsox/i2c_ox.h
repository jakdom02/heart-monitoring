#include "hardware/i2c.h"
#include "stdio.h"
#include "stdlib.h"

void bus_scan(void);

void max30100_reset(void);

void max30100_init(void);

void max30100_read_reg(uint8_t reg);

float max30100_read_temperature(void);

void max30100_read_fifo(void);

