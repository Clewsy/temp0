#include "hdc1080.h"

hdc1080::hdc1080(void)
{
//	Wire.begin();	// Communicating with the hdc1080 requires I2C.  Not needed here as wire is initialised in main.
}

// Read the current configuration register value.  Returns 8-bit integer representing current register state.
// Note, configuration register is actually 16 bits, but the lower 7 bits are all reserved (zero).
uint8_t hdc1080::read_config_register(void)
{
	Wire.beginTransmission(HDC1080_I2C_ADDRESS);
	Wire.write(HDC1080_ADDRESS_CONF);		// Tell the HDC1080 which register we want to read.
	Wire.endTransmission(HDC1080_I2C_ADDRESS);

	// Read the raw sensor data from the hdc1080.
	Wire.requestFrom(HDC1080_I2C_ADDRESS, 2, true);	// Provide I2C address, and requested number of bytes to read.
	uint8_t config_data = Wire.read();		// Read in the current config data byte.
	Wire.read();					// HDC1080 config register is 16 bytes, but lower byte is reserved (all zeros) so ignore.

	return config_data;
}

// Send a command to the configuration register.
void hdc1080::set_config_register(uint8_t reg)
{
	Wire.beginTransmission(HDC1080_I2C_ADDRESS);	// Begin an I2C transmission to I2C address of the hdc1080.
	Wire.write(HDC1080_ADDRESS_CONF);		// Write the address of the configuration register.
	Wire.write(reg);				// Write the desired configuration bits - config register high byte.
	Wire.write(0x00);				// Config register has a low byte which is not used but expected.  Write all zeros.
	Wire.endTransmission(HDC1080_I2C_ADDRESS);	// Signal the end of the I2C transmission.
}

// Trigger a soft reset of the hdc1080.
void hdc1080::reset(void)
{
	uint8_t reg = read_config_register();		// Set reg to current value of the config register.
	reg = (reg | (1 << HDC1080_CONFIG_BIT_RESET));	// Set reg to include high reset bit (self-clears after set).
	set_config_register(reg);			// Set the config register to the value of reg.
}

// Initialise the hdc1080.
void hdc1080::init(void)
{
	uint8_t reg = read_config_register();			// Set reg to current value of the config register.
	reg = (reg | 	(1 << HDC1080_CONFIG_BIT_MODE) |	// OR reg to include desired config bits: Set mode to transmit temperature and humidity data together. 
			(1 << HDC1080_CONFIG_BIT_T_RES) |	//					  Set temperature resolution to 11 bit.
			(1 << HDC1080_CONFIG_BIT_H_RES_MSB) );	//					  Set humidity resolution to 8 bit.
	set_config_register(reg);				// Set the config register to the value of reg.
}

// Get the temperature and humidity readings from the hdc1080.
double * hdc1080::get_sensor_data(void)
{
	// Trigger an update of the temerature and humidity readings in the hdc1080.
	Wire.beginTransmission(HDC1080_I2C_ADDRESS);
	Wire.write(HDC1080_ADDRESS_TRIG);
	Wire.endTransmission(HDC1080_I2C_ADDRESS);

	// Wait long enough for the hdc1080 to update the sensor data.
	delay(HDC1080_SENSE_DELAY);

	// Read the raw sensor data from the hdc1080.
	Wire.requestFrom(HDC1080_I2C_ADDRESS, 4, true);	// Provide I2C address, and requested number of bytes to read.
	temp_data = (Wire.read() << 8);			// Raw temperature data high byte.
	temp_data |= Wire.read();			// Raw temperature data low byte.
	humi_data = (Wire.read() << 8);			// Raw humidity data high byte.
	humi_data |= Wire.read();			// Raw humidity data low byte.

	// Convert the raw data into human readable values.
	temp = (((((double)temp_data)*165)/65536)-40);	// Convert float from the raw data: temp = (((temp_data / 2^16) * 165°C) - 40°C)
	humi = ((((double)humi_data)*100)/65536);	// Convert float from the raw data: humi = ((humi_data / 2^16) * 100%RH)

	// Copy the converted sensor data to the relevant data array elements. 
	data[TEMPERATURE] = temp;
	data[HUMIDITY] = humi;

	// Return the address of the "data" array.
	return data;
}

// Run the internal heater for a specified duration in seconds (approximately).
void hdc1080::run_heater(uint8_t seconds)
{
	uint8_t reg = read_config_register();		// Set reg to current value of the config register.
	reg = (reg | (1 << HDC1080_CONFIG_BIT_HEAT));	// Set reg to include heater enable bit.
	set_config_register(reg);			// Set the config register to the value of reg.

	// Heater only runs when in measurement mode so simulate for requested duration (approx timing):
	for (uint16_t i=0; i<(seconds*100); i++)
	{
		Wire.beginTransmission(HDC1080_I2C_ADDRESS);
		Wire.write(HDC1080_ADDRESS_TRIG);
		Wire.endTransmission(HDC1080_I2C_ADDRESS);
		delay(10);
		Wire.requestFrom(HDC1080_I2C_ADDRESS, 4, true);
		for (uint8_t j=0; j<5; j++) { Wire.read(); }
	}

	reg = (reg & ~(1 << HDC1080_CONFIG_BIT_HEAT));	// Clear the heater enable bit (i.e. disable heater).
	set_config_register(reg);			// Set the config register to the value of reg.
}
