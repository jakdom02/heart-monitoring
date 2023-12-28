#include "hardware/i2c.h"

// Write function declaration 
int reg_write(i2c_inst_t *i2c, const uint addr, const uint8_t reg, unit8_t *buf, const uint8_t nbytes);

// Read function declaration
int reg_read(i2c_inst_t *i2c, const uint addr, const uint8_t reg, unit8_t *buf, const uint8_t nbytes)