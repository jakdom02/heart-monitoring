#include "hardware/i2c.h"
#include "stdio.h"
#include "stdlib.h"


void bus_scan(void);

void read_isr(void);

void max30100_reset(void);

void max30100_init(void);

uint16_t max30100_read_temperature(void);

uint32_t max30100_read_fifo(void);

