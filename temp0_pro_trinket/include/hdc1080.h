#include <Arduino.h>
#include <Wire.h>




#define HDC1080_I2C_ADDRESS 0b1000000

#define HDC1080_ADDRESS_TRIG	0x00
#define HDC1080_ADDRESS_TEMP	0x00
#define HDC1080_ADDRESS_HUMI	0x01
#define HDC1080_ADDRESS_CONF	0x02

#define HDC1080_CONFIG_BIT_RESET	7	//Upper config byte; i.e. bit 15.
#define HDC1080_CONFIG_BIT_HEAT		5	//Upper config byte; i.e. bit 13.
#define HDC1080_CONFIG_BIT_MODE		4	//Upper config byte; i.e. bit 12.
#define HDC1080_CONFIG_BIT_BATT		3	//Upper config byte; i.e. bit 11.
#define HDC1080_CONFIG_BIT_T_RES	2	//Upper config byte; i.e. bit 10.
#define HDC1080_CONFIG_BIT_H_RES_MSB	1	//Upper config byte; i.e. bit 9.
#define HDC1080_CONFIG_BIT_H_RES_LSB	0	//Upper config byte; i.e. bit 8.


void hdc1080_reset(void);
void hdc1080_init(void);
void hdc1080_trigger_sensor(void);
float * hdc1080_get_sensor_data(void);