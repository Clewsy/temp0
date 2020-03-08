#include "hdc1080.h"

//#include <Wire.h>
/*
void hdc1080_reset(void)
{
	Wire.beginTransmission(HDC1080_I2C_ADDRESS);
	Wire.write(HDC1080_ADDRESS_CONF);
	Wire.write(1 << HDC1080_CONFIG_BIT_RESET);
	Wire.write(0x00);
	Wire.endTransmission(HDC1080_I2C_ADDRESS);
}

void hdc1080_init(void)
{
	hdc1080_reset();

	Wire.beginTransmission(HDC1080_I2C_ADDRESS);
	Wire.write(HDC1080_ADDRESS_CONF);
	Wire.write(	(1 << HDC1080_CONFIG_BIT_MODE) |
			(1 << HDC1080_CONFIG_BIT_T_RES) |
			(1 << HDC1080_CONFIG_BIT_H_RES_MSB) );
//	Wire.write(0b00010110);
	Wire.write(0x00);
	Wire.endTransmission(HDC1080_I2C_ADDRESS);

}

void hdc1080_trigger_sensor(void)
{
	Wire.beginTransmission(HDC1080_I2C_ADDRESS);
	Wire.write(HDC1080_ADDRESS_TRIG);
	Wire.endTransmission(HDC1080_I2C_ADDRESS);
}

float * hdc1080_get_sensor_data(void)
{
	hdc1080_trigger_sensor();
	delay(20);

	static float data[2];					//Address of array remains static.

	Wire.requestFrom(HDC1080_I2C_ADDRESS, 4);
	uint8_t temp_msb = Wire.read();
	uint8_t temp_lsb = Wire.read();
	uint8_t humi_msb = Wire.read();
	uint8_t humi_lsb = Wire.read();

	uint16_t temp_data = ( (temp_msb << 8) | temp_lsb);
	float temp = ((float)temp_data);
	temp=temp*165;
	temp= (temp / 65536);
	temp = (temp-40);

	uint16_t humi_data = ( (humi_msb << 8) | humi_lsb);
	float humi = ((float)humi_data);
	humi = humi * 100;
	humi = humi / 65536;

	data[0] = temp;
	data[1] = humi;

	return data;						//Returns address of array "data".

}*/



hdc1080::hdc1080(void) {}

void hdc1080::reset(void)
{
	Wire.beginTransmission(HDC1080_I2C_ADDRESS);
	Wire.write(HDC1080_ADDRESS_CONF);
	Wire.write(1 << HDC1080_CONFIG_BIT_RESET);
	Wire.write(0x00);
	Wire.endTransmission(HDC1080_I2C_ADDRESS);
}

void hdc1080::init(void)
{
	Wire.beginTransmission(HDC1080_I2C_ADDRESS);
	Wire.write(HDC1080_ADDRESS_CONF);
	Wire.write(	(1 << HDC1080_CONFIG_BIT_MODE) |
			(1 << HDC1080_CONFIG_BIT_T_RES) |
			(1 << HDC1080_CONFIG_BIT_H_RES_MSB) );
//	Wire.write(0b00010110);
	Wire.write(0x00);
	Wire.endTransmission(HDC1080_I2C_ADDRESS);
}

/*
void hdc1080::trigger_sensor(void)
{
	Wire.beginTransmission(HDC1080_I2C_ADDRESS);
	Wire.write(HDC1080_ADDRESS_TRIG);
	Wire.endTransmission(HDC1080_I2C_ADDRESS);
	delay(HDC1080_SENSE_DELAY);
}
*/

float * hdc1080::get_sensor_data(void)
{

	Wire.beginTransmission(HDC1080_I2C_ADDRESS);
	Wire.write(HDC1080_ADDRESS_TRIG);
	Wire.endTransmission(HDC1080_I2C_ADDRESS);
	delay(HDC1080_SENSE_DELAY);


	static float data[2];					//Address of array remains static.

	Wire.requestFrom(HDC1080_I2C_ADDRESS, 4);
	uint8_t temp_msb = Wire.read();
	uint8_t temp_lsb = Wire.read();
	uint8_t humi_msb = Wire.read();
	uint8_t humi_lsb = Wire.read();

	uint16_t temp_data = ( (temp_msb << 8) | temp_lsb);
	float temp = ((float)temp_data);
	temp=temp*165;
	temp= (temp / 65536);
	temp = (temp-40);

	uint16_t humi_data = ( (humi_msb << 8) | humi_lsb);
	float humi = ((float)humi_data);
	humi = humi * 100;
	humi = humi / 65536;

	data[0] = temp;
	data[1] = humi;

	return data;						//Returns address of array "data".

}