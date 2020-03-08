#include "hdc1080.h"

hdc1080::hdc1080(void)
{
	Wire.begin();	//Communicating with the hdc1080 requires I2C.
}

//Trigger a soft reset of the hdc1080.
void hdc1080::reset(void)
{
	//Begin an I2C transmission to I2C address of the hdc1080.
	Wire.beginTransmission(HDC1080_I2C_ADDRESS);

	//Write the address of the configuration register.
	Wire.write(HDC1080_ADDRESS_CONF);

	//Write the desired configuration bits - config register high byte.
	Wire.write(1 << HDC1080_CONFIG_BIT_RESET);	//Set the hdc1080 reset bit.  Will automatically reset.

	//Config register has a low byte which is not used but expected.  Write all zeros.
	Wire.write(0x00);

	//Signal the end of the I2C transmission.
	Wire.endTransmission(HDC1080_I2C_ADDRESS);
}

//Initialise the hdc1080.
void hdc1080::init(void)
{
	//Begin an I2C transmission to I2C address of the hdc1080.
	Wire.beginTransmission(HDC1080_I2C_ADDRESS);

	//Write the address of the configuration register.
	Wire.write(HDC1080_ADDRESS_CONF);

	//Write the desired configuration bits - config register high byte.
	Wire.write(	(1 << HDC1080_CONFIG_BIT_MODE) |	//Set mode to transmit temperature and humidity data together.
			(1 << HDC1080_CONFIG_BIT_T_RES) |	//Set temperature resolution to 11 bit.
			(1 << HDC1080_CONFIG_BIT_H_RES_MSB) );	//Set humidity resolution to 8 bit.

	//Config register has a low byte which is not used but expected.  Write all zeros.
	Wire.write(0x00);

	//Signal the end of the I2C transmission.
	Wire.endTransmission(HDC1080_I2C_ADDRESS);
}

//Get the temperature and humidity readings from the hdc1080.
float * hdc1080::get_sensor_data(void)
{
	//Trigger an update of the temerature and humidity readings in the hdc1080.
	Wire.beginTransmission(HDC1080_I2C_ADDRESS);
	Wire.write(HDC1080_ADDRESS_TRIG);
	Wire.endTransmission(HDC1080_I2C_ADDRESS);

	//Wait long enough for the hdc1080 to update the sensor data.
	delay(HDC1080_SENSE_DELAY);

	//Read the raw sensor data from the hdc1080.
	Wire.requestFrom(HDC1080_I2C_ADDRESS, 4);	//Provide I2C address, and requested number of bytes to read.
	temp_data = ( Wire.read() << 8);		//Raw temperature data high byte.
	temp_data |= Wire.read();			//Raw temperature data low byte.
	humi_data = ( Wire.read() << 8);		//Raw humidity data high byte.
	humi_data |= Wire.read();			//Raw humidity data low byte.

	//Convert the raw data into human readable values.
	temp = (((((float)temp_data)*165)/65536)-40);	//Convert float from the raw data: temp = (((temp_data / 2^16) * 165°C) - 40°C)
	humi = ((((float)humi_data)*100)/65536);	//Convert float from the raw data: humi = ((humi_data / 2^16) * 100%RH)

	//Copy the converted sensor data to the relevant data array elements. 
	data[TEMPERATURE] = temp;
	data[HUMIDITY] = humi;

	//Return the address of the "data" array.
	return data;
}