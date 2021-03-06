#include <Arduino.h>
#include <Wire.h>		// Needed for I2C communication.
#include "ssd1306_fonts.h"	// Defines font maps.
#include "ssd1306_bitmaps.h"	// Defines image bitmaps.

#define OLED_ADDR			0b0111100	// i2c address of the OLED SSD1306 driver module.
#define OLED_CONTROL_BYTE_COMMAND 	0b10000000	// Control byte - indicates that the next byte will be a command.
#define OLED_CONTROL_BYTE_DATA		0b11000000	// Control byte - indicates that the next byte will be "data" (i.e. graphical data).
#define OLED_CONTROL_BYTE_BULK_DATA	0b01000000	// Control byte - indicates that the transmission will contain only data bytes.

//SSD1306 command bytes.
//Fundamental Commands:
#define OLED_SET_CONTRAST				0x81	// Set the contrast.  Following command byte defines contrast value from 0 (low) to 255 (high). (0x7F default).
#define OLED_SET_CONTRAST_DEFAULT			0x7F	// Range from 0x00 (dimest) to 0xFF (brightest).
#define OLED_SET_CONTRAST_BRIGHT			0xFF	// Highest contrast (brightest).
#define OLED_ALL_ON_RESUME				0xA4	// Turn on all pixels.
#define OLED_ALL_ON					0xA5	// Disable "OLED_ALL_ON" (i.e. resume previous graphical output).
#define OLED_INVERSE_DISABLE				0xA6	// Normal mode - setting segment bits turns pixels on.
#define OLED_INVERSE_ENABLE				0xA7	// Inverse mode - setting segment bits turns pixels off.
#define OLED_ON 					0xAF	// Turn on (enable) the oled module.
#define OLED_OFF					0xAE	// Turn off (disable) the oled module.
//Hardware Configuration Commands:
#define OLED_SET_DISPLAY_START_LINE			0x40	// Set the display start line 0-63.  0x40: line 0 (default).  OR with 6 bits up to 0b111111 for line 63.
#define OLED_SET_SEGMENT_REMAP_COL0			0xA0	// Column address 0 is mapped to SEG 0 (default).
#define OLED_SET_SEGMENT_REMAP_COL127			0xA1	// Column address 127 is mapped to SEG 0. 
#define OLED_SET_MULTIPLEX_RATIO			0xA8	// Set the MUX ratio to N+1 MUX.  Next byte defines MUX from 16-64.  (Default is 0b111111 i.e. 64MUX).
#define OLED_SET_MULTIPLEX_RATIO_DEFAULT		0x3F	// I.e. 64MUX.
#define OLED_SET_COM_OUTPUT_SCAN_DIR_NORMAL		0xC0	// Scan from COM0 to COM[N-1] (N is MUX ratio) (default).
#define OLED_SET_COM_OUTPUT_SCAN_DIR_REVERSE		0xC8	// Scan from COM[N-1] to COM0 (N is MUX ratio).
#define OLED_SET_DISPLAY_OFFSET				0xD3	// Set vertical shift by COM from 0 to 63 defined by next byte.  Default is 0.
#define OLED_SET_DISPLAY_OFFSET_DEFAULT			0x00	// I.e. 0 offset.
#define OLED_SET_COM_PINS_HARDWARE_CONFIG		0xDA	// Set COM pin config with next byte.  Default is 0b00100010 - Alternative COM pin config, disable COM left/right remap.
#define OLED_SET_COM_PINS_HARDWARE_CONFIG_DEFAULT	0x12	// I.e. alternative COM pin config, disable COM left/right remap.
//Timing and Driving Scheme Commands:
#define OLED_SET_DISPLAY_CLOCK				0xD5	// Set the clock divide ratio/oscillator frequency.  Next byte [7-4] osc freq, [3-0] divide ratio (n +1). 
#define OLED_SET_DISPLAY_CLOCK_DEFAULT			0x80	// I.e. default settings osc freq 0b10000, divide ratio 1.
#define OLED_SET_CHARGE_PUMP				0x8D	// Enable or disable inetrnal charge pump regulator with the next byte.
#define OLED_SET_CHARGE_PUMP_DEFAULT			0x14	// Enable by default - 0x14.  (To disable, use 0x10 to disable).
#define OLED_SET_PRECHARGE_PERIOD			0xD9	// Set pre-charge period with next byte.  Default is 0b10000000 = 0x80
#define OLED_SET_PRECHARGE_PERIOD_DEFAULT		0x22	// I.e. Phase 1 period 0x2, phase 2 period 0x2.
#define OLED_SET_VCOMH_DESELECT_LEVEL			0xDB	// Set Vcomh deselect voltage level with next byte (default 0b00100000 = 0x20).
#define OLED_SET_VCOMH_DESELECT_LEVEL_DEFAULT		0x20	// I.e. ~0.77xVcc.
#define OLED_NO_OP					0xE3	// Command for no operation.
//Addressing Setting Commands:
#define OLED_SET_MEMORY_ADDRESSING_MODE			0x20	// Set mode of addressing oled pixel data with next byte (default is 0b10 for page mode).
#define OLED_SET_MEMORY_ADDRESSING_MODE_DEFAULT		0x02	// I.e Page addressing mode.
#define OLED_SET_MEMORY_ADDRESSING_MODE_HORIZONTAL	0x00	// Horizontal addressing mode automnatically rolls index down through pages.
#define OLED_ADDRESS_FIRST_COLUMN_LOWER			0x00	// (Page Mode Only) Command to set lower nibble of column address to first column.  Add lower nibble of column 0 to 127.
#define OLED_ADDRESS_FIRST_COLUMN_HIGHER		0x10	// (Page Mode Only) Command to set higher nibble of column address to first column.  Or with higher nibble of column 0 to 127.
#define OLED_ADDRESS_FIRST_PAGE				0xB0	// (Page Mode Only) Command to set address to first page.  Or with 0 to 7 to set address to pages 0 to 7.
#define OLED_ADDRESS_COLUMN				0x21	// (Horizontal Mode) Following two bytes set start and end columns 0 to 127.  Default 0 then 127.
#define OLED_ADDRESS_PAGE				0x22	// (Horizontal Mode) Following two bytes set start and end pages 0 to 7.  Default 0 then 7.

// Define class for an oled display with onboard ssd1306 driver.
class ssd1306
{
	public:
		void init(void);
		void invert_screen(bool invert);
		void draw_box(uint8_t start_page, uint8_t start_column, uint8_t height, uint8_t width);
		void map_bits(const uint8_t *bitmap, const uint16_t bitmap_size);
		void test_pattern(void);
		void print_char(unsigned char character, const uint8_t *font, uint8_t start_page, uint8_t start_column);
		void print_string(unsigned char *string, const uint8_t *font, uint8_t start_page, uint8_t start_column);

	private:
		void send_command(uint8_t command);
		void send_command(uint8_t command1, uint8_t command2);
		void send_command(uint8_t command1, uint8_t command2, uint8_t command3);
		void set_page(uint8_t page);
		void set_column(uint8_t column);
		void set_address(uint8_t page, uint8_t column);
		void send_data(uint8_t data);
		void send_segment(uint8_t byte, uint8_t page, uint8_t column);
};
