#include <Arduino.h>
#include <Wire.h>		// Needed for I2C communication.

#define TEMPERATURE	0	// Sensor array element to contain temperature value.
#define HUMIDITY	1	// Sensor array element to contain humidity value.

#define HDC1080_I2C_ADDRESS 0b1000000	// Address of the hdc1080 on the I2C bus.

#define HDC1080_ADDRESS_TRIG	0x00	// Address of "trigger" register - same as temp register but used when mode bit is set.
#define HDC1080_ADDRESS_TEMP	0x00	// Address of the temperature raw data register.
#define HDC1080_ADDRESS_HUMI	0x01	// Address of the humidity raw data register.
#define HDC1080_ADDRESS_CONF	0x02	// Address of the hdc1080 configuration register.

#define HDC1080_CONFIG_BIT_RESET	7	// Upper config byte; i.e. bit 15.	Set to reset device.
#define HDC1080_CONFIG_BIT_HEAT		5	// Upper config byte; i.e. bit 13.	Set to enable the internal heater.
#define HDC1080_CONFIG_BIT_MODE		4	// Upper config byte; i.e. bit 12.	Set to transmit temperature and humidity data together (not separately).
#define HDC1080_CONFIG_BIT_BATT		3	// Upper config byte; i.e. bit 11.	Battery status, high if <2.8V (read only).
#define HDC1080_CONFIG_BIT_T_RES	2	// Upper config byte; i.e. bit 10.	Set for temperature 11bit resolution (not 14bit).
#define HDC1080_CONFIG_BIT_H_RES_MSB	1	// Upper config byte; i.e. bit 9.	Humidity resolution MSB:LSB:
#define HDC1080_CONFIG_BIT_H_RES_LSB	0	// Upper config byte; i.e. bit 8.	00-14bit, 01-11bit, 10-8bit

#define HDC1080_SENSE_DELAY 7	// Delay (in ms) to wait after triggering an update of hdc1080 sensor data, before reading in the data.  Refer to datasheet.

// Define class for an hdc1080 temperature/humidity sensor.
class hdc1080
{
	public:
		hdc1080(void);				// To initialise an hdc1080 sensor.
		void reset(void);			// Send a reset signal to the hdc1080.
		void init(void);			// Initialise an hdc1080.
		double * get_sensor_data(void);		// Get the temperature and humidty readings from an hdc1080.
		void run_heater(uint8_t seconds);	// Run the internal sensor heater for a specified duration.
	
	private:
		double sensor_values[2];		// Array with two elements that will contain floats for temperature and humidity.
		uint16_t temp_data;			// The raw temperature data from the hdc1080.
		uint16_t humi_data;			// The raw humidity data from the hdc1080.
		uint8_t read_config_register(void);	// Returns 8-bits representing current state of the config register.
		void set_config_register(uint8_t reg);	// Write 8-bits to the configuration register (upper byte of 16-bits).
};