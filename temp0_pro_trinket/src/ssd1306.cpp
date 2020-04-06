#include "ssd1306.h"

// Initiate an oled module.
ssd1306::ssd1306(void)
{
//	Wire.begin();	// Communicating with the ssd1306 requires I2C.  Not needed here as wire is initialised in main.
}

// Send a command to oled driver - single command only.
void ssd1306::send_command(uint8_t command)
{
	Wire.beginTransmission(OLED_ADDR);
	Wire.write(OLED_CONTROL_BYTE_COMMAND);
	Wire.write(command);
	Wire.endTransmission(OLED_ADDR);
}

// Send a command to oled driver - single command followed by command value.
void ssd1306::send_command(uint8_t command, uint8_t value)
{
	Wire.beginTransmission(OLED_ADDR);
	Wire.write(OLED_CONTROL_BYTE_COMMAND);
	Wire.write(command);
	Wire.write(OLED_CONTROL_BYTE_COMMAND);
	Wire.write(value);
	Wire.endTransmission(OLED_ADDR);
}

// Send a command to oled driver - single command followed by two command value.
void ssd1306::send_command(uint8_t command, uint8_t value1, uint8_t value2)
{
	Wire.beginTransmission(OLED_ADDR);
	Wire.write(OLED_CONTROL_BYTE_COMMAND);
	Wire.write(command);
	Wire.write(OLED_CONTROL_BYTE_COMMAND);
	Wire.write(value1);
	Wire.write(OLED_CONTROL_BYTE_COMMAND);
	Wire.write(value2);
	Wire.endTransmission(OLED_ADDR);
}

// Send a data byte - i.e. 8-bits to set/clear the 8 pixels at the current address.
void ssd1306::send_data(uint8_t data)
{
	Wire.beginTransmission(OLED_ADDR);
	Wire.write(OLED_CONTROL_BYTE_DATA);
	Wire.write(data);
	Wire.endTransmission(OLED_ADDR);
}

// In page addressing mode, valid pages are 0 to 7 (top to bottom).
void ssd1306::set_page(uint8_t page)
{
	send_command(OLED_ADDRESS_FIRST_PAGE + page);
}

// In page addressing mode, valid pages are 0 to 127 (left to right).
void ssd1306::set_column(uint8_t column)
{
	send_command(OLED_ADDRESS_FIRST_COLUMN_LOWER | (column & 0b1111), OLED_ADDRESS_FIRST_COLUMN_HIGHER | ((column >> 4)));
}

// Set the page then column address to be set by the next data write.
// Note, each data write increments the column automatically, but does not change the page.
void ssd1306::set_address(uint8_t page, uint8_t column)
{
	set_page(page);
	set_column(column);
}

// Write all zeros to oled data to clear all pixels.
void ssd1306::clear_screen(void)
{
	send_command(OLED_SET_MEMORY_ADDRESSING_MODE, OLED_SET_MEMORY_ADDRESSING_MODE_HORIZONTAL);	// Temporarily set memory addressing mode to Horizontal Mode.
	send_command(OLED_ADDRESS_PAGE, 0, 7);								// Set page address range.
	send_command(OLED_ADDRESS_COLUMN, 0, 127);							// Set column address range.
	
	for (uint8_t i=0; i<64; i++)	// Can send packets of maximum 16 bytes at a time.  Therefore need to repeat 64 times for all segments.
	{
		Wire.beginTransmission(OLED_ADDR);
		Wire.write(OLED_CONTROL_BYTE_BULK_DATA);
		for (uint8_t j=0; j<16; j++)
		{
			Wire.write(0x00);			// Segment index increases every write.
		}
		Wire.endTransmission(OLED_ADDR);
	}
	send_command(OLED_SET_MEMORY_ADDRESSING_MODE, OLED_SET_MEMORY_ADDRESSING_MODE_DEFAULT);		// Reset memory addressing mode back to Page Mode.
}

// Draw a box with top left corner at upper pixel of [start page] and column [start_column],
// with dimensions [width] pixels wide by [height]x8 pixels height (i.e. height is in incrments of page = 8-bits).
void ssd1306::draw_box(uint8_t start_page, uint8_t start_column, uint8_t height, uint8_t width)
{
	set_address(start_page, start_column);					// Location of top left box corner.

	send_data(0xFF);							// Left most column, first page. 

	for(uint8_t c = start_column; c < (start_column+width-2); c++)		// Run through top row first page for all columns.
	{
		if(height>1)	{send_data(0b00000001);}			// Top row only.
		else		{send_data(0b10000001);}			// Top and bottom fows.
	}

	send_data(0xFF);							// Right most column, first page.

	if(height>1)								// More than a single page.
	{
		for(uint8_t p = (start_page+1); p < (start_page + height); p++)	// Run through left- and right-most column for all pages between first and last.
		{
			set_address(p, start_column);				// Left column.
			send_data(0xFF);
			set_address(p, (start_column+width-1));			// Right column.
			send_data(0xFF);
		}
		set_address((start_page+height-1), (start_column));		// Run through bottom row last page for all columns.
		send_data(0xFF);
		for(uint8_t c = start_column; c < (start_column+width-2); c++)
		{
			send_data(0b10000000);					// Bottom row only.
		}
		send_data(0xFF);						// Right most column, last page. 
	}
}

// Initialise the oled display.
void ssd1306::init(void)
{
	send_command(OLED_SET_MULTIPLEX_RATIO, OLED_SET_MULTIPLEX_RATIO_DEFAULT);			// Set MUX ratio.
	send_command(OLED_SET_DISPLAY_OFFSET, OLED_SET_DISPLAY_OFFSET_DEFAULT);				// Set display offset.
	send_command(OLED_SET_DISPLAY_START_LINE);							// Set display start line.
	send_command(OLED_SET_SEGMENT_REMAP_COL0);							// Set segment re-map.  COL 0 is default.
//	send_command(OLED_SET_SEGMENT_REMAP_COL127);							// Set segment re-map.  COL 0 is default.
	send_command(OLED_SET_COM_OUTPUT_SCAN_DIR_NORMAL);						// Set COM output scan direction.  Normal is default.
//	send_command(OLED_SET_COM_OUTPUT_SCAN_DIR_REVERSE);						// Set COM output scan direction.  Normal is default.
	send_command(OLED_SET_COM_PINS_HARDWARE_CONFIG, OLED_SET_COM_PINS_HARDWARE_CONFIG_DEFAULT);	// Set COM pins hardware configuration.
	send_command(OLED_SET_CONTRAST, OLED_SET_CONTRAST_DEFAULT);					// Set contrast control.
	send_command(OLED_ALL_ON_RESUME);								// Disable entire display on.
	send_command(OLED_INVERSE_DISABLE);								// Set Normal display (i.e. not inverse).
	send_command(OLED_SET_DISPLAY_CLOCK, OLED_SET_DISPLAY_CLOCK_DEFAULT);				// Set Osc frequency.
	send_command(OLED_SET_CHARGE_PUMP, OLED_SET_CHARGE_PUMP_DEFAULT);				// Enable charge pump regulator.
	send_command(OLED_SET_PRECHARGE_PERIOD, OLED_SET_PRECHARGE_PERIOD_DEFAULT);			// Set charge pump pre-charge period.
	send_command(OLED_SET_MEMORY_ADDRESSING_MODE, OLED_SET_MEMORY_ADDRESSING_MODE_DEFAULT);		// Set memory addressing mode to Page Addressing Mode.
	clear_screen();											// Clear any remnant screen memory from last run.
	send_command(OLED_ON);										// Turn display on.
}

// Print a test patter to the display.
void ssd1306::test_pattern(void)
{
	send_command(OLED_SET_MEMORY_ADDRESSING_MODE, OLED_SET_MEMORY_ADDRESSING_MODE_HORIZONTAL);	// Temporarily set memory addressing mode to Horizontal Mode.
	send_command(OLED_ADDRESS_PAGE, 0, 7);								// Set page address range.
	send_command(OLED_ADDRESS_COLUMN, 0, 127);							// Set column address range.
	
	for (uint8_t i=0; i<64; i++)	// Can send packets of maximum 16 bytes at a time.  Therefore need to repeat 64 times for all segments.
	{
		Wire.beginTransmission(OLED_ADDR);
		Wire.write(OLED_CONTROL_BYTE_BULK_DATA);
		for (uint8_t j=0; j<16; j++)
		{
			Wire.write(0b11110000);		// Segment index increases every write.
		}
		Wire.endTransmission(OLED_ADDR);
		delay(10);				// Delay for dramatic effect.
	}
	send_command(OLED_SET_MEMORY_ADDRESSING_MODE, OLED_SET_MEMORY_ADDRESSING_MODE_DEFAULT);		// Reset memory addressing mode back to Page Mode.
}

//////////////////////////////////////////////////////////////////////////////////////////////

// Actually send a segment (8-pixel column) to the ssd1306.
// TO ADD: ERROR-CHECKING - don't want to send segment if out of bounds of the display.
void ssd1306::send_segment(uint8_t byte, uint8_t page, uint8_t column)
{
	set_address(page, column);
	send_data(byte);
}

// Take a character, obtain the character map data from the defined font, and print that character to the defined co-ordinates.
void ssd1306::print_char(unsigned char character, const uint8_t *font, uint8_t start_page, uint8_t start_column)
{

//	uint8_t font_width = pgm_read_byte(&&font[FONT_WIDTH]));		// From the font metadata obtain width of characters in pixels (this is max width for non fixed-width fonts).
	uint8_t font_height = pgm_read_byte(&font[FONT_HEIGHT]);		// From the font metadata obtain height of characters in pixels.
	uint8_t font_first_char = pgm_read_byte(&font[FONT_FIRST_CHAR]);	// From the font metadata obtain the value of the first included character (often ' ' (i.e. space) or 0x20).
	uint8_t font_num_chars = pgm_read_byte(&font[FONT_NUM_CHARS]);		// From the font metadata obtain the total number of characters included.

	// Determine address of character data for the current character.  This is a 16-bit value split into two 8-bit values stored in PROGMEM.
	// Character address MSB is the byte located at ( FIRST_CHAR_META + ((character - font_first_char) * 4) + CHAR_ADDR_MSB )
	// Note the 16-bit address is indexed from zero where zero is the location of the first character data byte.
	// Therefore actual array address neads to be incremented by the number of bytes used by all metadata.
	//	FIRST_CHAR_META : address of the first character-specific metadata (i.e. skip over the font metadata bytes)
	//	character-font_first_char : convert the char value to the character index number.  E.g. for char '!', 0x21-0x20=0x01, therefore second character in index (0x00 is first).
	//	*4 : Each character has four metadata bytes, so want to skip over 4*character index bytes to get to the metadata for the current character.
	//	+ CHAR_ADDR_MSB : Within the character metadata, the MSB of the address is at index CHAR_ADDR_MSB.
	uint16_t char_data_addr =	( (pgm_read_byte(&font[FIRST_CHAR_META + ((character-font_first_char) * 4) + CHAR_ADDR_MSB]) ) << 8	) +	// Character index MSB shifted left 8 bits +
					( (pgm_read_byte(&font[FIRST_CHAR_META + ((character-font_first_char) * 4) + CHAR_ADDR_LSB]) )		) +	// Character index LSB +
					( FIRST_CHAR_META + (font_num_chars * 4 ) );									// Number of bytes used by font and character metadata.

	uint8_t char_data_size = pgm_read_byte(&font[ FIRST_CHAR_META + ((character-font_first_char) * 4) + CHAR_BYTE_SIZE ]);	// From the char metadata obtain the number of data bytes that define the char.
	uint8_t char_width = pgm_read_byte(&font[ FIRST_CHAR_META + ((character-font_first_char) * 4) + CHAR_WIDTH ]);		// From the char metadata obtain the width in pixels. 

	// Determine the minimum number of pages needed to display the full height of the character - pages are 8 pixels tall.
	uint8_t char_pages;
	if (font_height % 8)	{char_pages = ((font_height+(8-(font_height % 8))) / 8);}
	else			{char_pages = (font_height / 8);}
	
	// Run through each of the bytes of character data and send to the correct segment address.
	//	Page address : start_page + (b % char_pages)		: Starting from the top page, move down with each increment for the number of pages needed (char_pages), then roll back to the top page.
	//	Column address : start_column + (b / char_pages) 	: Starting from the left-most column, remain on that column for the required number of pages then move to the next column.
	for (uint8_t b = 0; b < char_data_size; b++)
	{
		send_segment(pgm_read_byte(&font[char_data_addr]), ( start_page + (b % char_pages) ), ( start_column + (b / char_pages) ));

		char_data_addr++;	// Increment the working address to the next byte in the character data.
	}

	// Font data includes the minimum bytes needed without 0x00 bytes at the end.  Now need to send these 0x00 bytes to clear the remaining character frame.
	//	Number of these padding bytes calculated by minusing the number of character data bytes (char_data_size) from the total number of bytes needed.
	//	Total number of bytes needed calculated from the character width (char_width) multiplied by the number of pages needed for the character height (char_pages).
	for (uint8_t b = char_data_size; b < (char_pages * char_width); b++)
	{
		send_segment(0x00, ( start_page + (b % char_pages) ), ( start_column + (b / char_pages) ));
	}
}

// Sequentially print each character in a string starting at the defined co-ordinates.
void ssd1306::print_string(unsigned char *string, const uint8_t *font, uint8_t start_page, uint8_t start_column)
{
	uint8_t font_first_char = pgm_read_byte(&font[FONT_FIRST_CHAR]);	// From the font metadata obtain the value of the first included character (often ' ' (i.e. space) or 0x20).

	uint8_t column = start_column;	// Initialise the working column - starts at start_column.

	uint8_t character = 0;		// Initialise the character index to be used in the character array.	
	while(string[character])	// Repeat the while loop until "character" is incremented to the end of the string.
	{
		uint8_t current_char_width = pgm_read_byte(&font[ FIRST_CHAR_META + ((string[character]-font_first_char) * 4) + CHAR_WIDTH ]);	// From the character metadata determine width in pixels of the current char.

		print_char(string[character], font, start_page, column);										// Print the current character to the oled.

		column+=current_char_width;												// Increment the column index by the width of the character.

		character++;														// Increment to the next character in the string.
	}
}

// Convert a double to an ascii srting, then print that string to the defined co-ordinates.
void ssd1306::print_double(double number, const uint8_t *font, uint8_t start_page, uint8_t start_column)
{
	char buffer[6];					// Define an array buffer to store the ascii string.
	dtostrf(number, 5, 2, buffer);			// Double-to-String function.  Syntax dtostrf(double, min_string_length, num_post_decimal_values, char_array_addr).

	print_string((unsigned char *) buffer, font, start_page, start_column);	// Send the string to the print_string function.
}
