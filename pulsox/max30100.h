/**
 * @file max30100.h
 * @author T.Mroz J.Domagala
 */

#include "hardware/i2c.h"
#include "stdio.h"
#include "stdlib.h"

#define ONE_BIT_FIELD   (1)
#define TWO_BIT_FIELD   ((1 << 1) | (1 << 0))
#define THREE_BIT_FIELD   ((1 << 2) | (1 << 1) | (1 << 0))
#define FOUR_BIT_FIELD   ((1 << 3) | (1 << 2) | (1 << 1) | (1 << 0))

#define MAX30100_I2C_ADDR       (0x57)

#define MAX30100_ISR            (0x00)
#define MAX30100_ISR_A_FULL     (ONE_BIT_FIELD << 7)
#define MAX30100_ISR_TEMP_RDY   (ONE_BIT_FIELD << 6)
#define MAX30100_ISR_HR_RDY     (ONE_BIT_FIELD << 5)
#define MAX30100_ISR_SPO2_RDY   (ONE_BIT_FIELD << 4)
#define MAX30100_ISR_PWR_RDY    (ONE_BIT_FIELD << 0)

#define MAX30100_IER                (0x01)
#define MAX30100_IER_ENB_A_FUL      (ONE_BIT_FIELD << 7)
#define MAX30100_IER_ENB_TEMP_RDY   (ONE_BIT_FIELD << 6)
#define MAX30100_IER_ENB_HR_RDY     (ONE_BIT_FIELD << 5)
#define MAX30100_IER_ENB_SPO2_RDY   (ONE_BIT_FIELD << 4)

#define MAX30100_FIFO_WRR            (0x02)
#define MAX30100_FIFO_WRR_PTR        (FOUR_BIT_FIELD << 0)

#define MAX30100_FIFO_OVFR           (0x03)
#define MAX30100_FIFO_OVFR_COUNTER   (FOUR_BIT_FIELD << 0)

#define MAX30100_FIFO_RDR            (0x04)
#define MAX30100_FIFO_RDR_PTR        (FOUR_BIT_FIELD << 0)

#define MAX30100_FIFO_DATAR          (0x05)

#define MAX30100_MODE_CONFR          (0x06)
#define MAX30100_MODE_CONFR_SHDN     (ONE_BIT_FIELD << 7)
#define MAX30100_MODE_CONFR_RESET    (ONE_BIT_FIELD << 6)
#define MAX30100_MODE_CONFR_TEMP_EN  (ONE_BIT_FIELD << 3)
#define MAX30100_MODE_CONFR_MODE     (THREE_BIT_FIELD << 0)

#define MAX30100_SPO2_CONFR                 (0x07)
#define MAX30100_SPO2_CONFR_HI_RES_EN       (ONE_BIT_FIELD << 6)
#define MAX30100_SPO2_CONFR_SPO2_SR         (THREE_BIT_FIELD << 2)
#define MAX30100_SPO2_CONFR_SPO2_SR_OFFSET  (2)
#define MAX30100_SPO2_CONFR_LED_PW          (TWO_BIT_FIELD << 0)

#define MAX30100_LED_CONFR                  (0x09)
#define MAX30100_LED_CONFR_RED_PA           (FOUR_BIT_FIELD << 4)
#define MAX30100_LED_CONFR_RED_PA_OFFSET    (4)
#define MAX30100_LED_CONFR_IR_PA            (FOUR_BIT_FIELD << 0)
#define MAX30100_LED_CONFR_IR_PA_OFFSET     (0)

#define MAX30100_TINTR                      (0x16)

#define MAX30100_TFRACR                     (0x17)
#define MAX30100_TFRACR_TFRAC               (FOUR_BIT_FIELD << 0)

#define MAX30100_REV_ID                     (0xFE)
#define MAX30100_PART_ID                    (0xFF)

#define MAX30100_MODE_HR_ONLY               (ONE_BIT_FIELD << 1)
#define MAX30100_MODE_SPO2                  (TWO_BIT_FIELD << 0)

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

void bus_scan(void);

void read_isr(uint8_t* dataPtr);

void max30100_reset(void);

void max30100_init(void);

void max30100_read_temperature(uint8_t* integer, uint8_t* fraction);

void max30100_read_fifo(uint8_t* dataPtr);

