#include "ssd1306.h"

// Send a command to oled driver - single command only.
void ssd1306::send_command(uint8_t command)
{
	Wire.beginTransmission(OLED_ADDR);
	Wire.write(OLED_CONTROL_BYTE_COMMAND);
	Wire.write(command);
	Wire.endTransmission(OLED_ADDR);
}

// Send a command to oled driver - single command followed by command value or a second command.
void ssd1306::send_command(uint8_t command1, uint8_t command2)
{
	Wire.beginTransmission(OLED_ADDR);
	Wire.write(OLED_CONTROL_BYTE_COMMAND);
	Wire.write(command1);
	Wire.write(OLED_CONTROL_BYTE_COMMAND);
	Wire.write(command2);
	Wire.endTransmission(OLED_ADDR);
}

// Send a command to oled driver - single command followed by two command values.  Or three serparate commands.
void ssd1306::send_command(uint8_t command1, uint8_t command2, uint8_t command3)
{
	Wire.beginTransmission(OLED_ADDR);
	Wire.write(OLED_CONTROL_BYTE_COMMAND);
	Wire.write(command1);
	Wire.write(OLED_CONTROL_BYTE_COMMAND);
	Wire.write(command2);
	Wire.write(OLED_CONTROL_BYTE_COMMAND);
	Wire.write(command3);
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

// Send a data byte - i.e. 8-bits to set/clear the 8 pixels at the current address.
void ssd1306::send_data(uint8_t data)
{
	Wire.beginTransmission(OLED_ADDR);
	Wire.write(OLED_CONTROL_BYTE_DATA);
	Wire.write(data);
	Wire.endTransmission(OLED_ADDR);
}

// Send a segment (8-pixel column) to the ssd1306.
void ssd1306::send_segment(uint8_t byte, uint8_t page, uint8_t column)
{
	// Don't bother sending data if the address is out-of-bounds.
	// Note, page & column are 8-bit unsigned.  Rolling over from 255->0 will be sent.
	if((page < 8) && (column < 128))
	{
		set_address(page, column);
		send_data(byte);
	}
}

/////////////////////////////////////// Begin public functions

// Initialise the oled display.
void ssd1306::init(void)
{
	send_command(OLED_SET_MULTIPLEX_RATIO, OLED_SET_MULTIPLEX_RATIO_DEFAULT);			// Set MUX ratio.
	send_command(OLED_SET_DISPLAY_OFFSET, OLED_SET_DISPLAY_OFFSET_DEFAULT);				// Set display offset.
	send_command(OLED_SET_DISPLAY_START_LINE);							// Set display start line.
//	send_command(OLED_SET_SEGMENT_REMAP_COL0);							// Set segment re-map.  COL 0 is default.
	send_command(OLED_SET_SEGMENT_REMAP_COL127);							// Set segment re-map.  COL 0 is default.
//	send_command(OLED_SET_COM_OUTPUT_SCAN_DIR_NORMAL);						// Set COM output scan direction.  Normal is default.
	send_command(OLED_SET_COM_OUTPUT_SCAN_DIR_REVERSE);						// Set COM output scan direction.  Normal is default.
	send_command(OLED_SET_COM_PINS_HARDWARE_CONFIG, OLED_SET_COM_PINS_HARDWARE_CONFIG_DEFAULT);	// Set COM pins hardware configuration.
	send_command(OLED_SET_CONTRAST, OLED_SET_CONTRAST_DEFAULT);					// Set contrast control.
	send_command(OLED_ALL_ON_RESUME);								// Disable entire display on.
	send_command(OLED_INVERSE_DISABLE);								// Set Normal display (i.e. not inverse).
	send_command(OLED_SET_DISPLAY_CLOCK, OLED_SET_DISPLAY_CLOCK_DEFAULT);				// Set Osc frequency.
	send_command(OLED_SET_CHARGE_PUMP, OLED_SET_CHARGE_PUMP_DEFAULT);				// Enable charge pump regulator.
	send_command(OLED_SET_PRECHARGE_PERIOD, OLED_SET_PRECHARGE_PERIOD_DEFAULT);			// Set charge pump pre-charge period.
	send_command(OLED_SET_MEMORY_ADDRESSING_MODE, OLED_SET_MEMORY_ADDRESSING_MODE_DEFAULT);		// Set memory addressing mode to Page Addressing Mode.
	send_command(OLED_ON);										// Turn display on.
}

// Sending true or a one to this function will cause the display to be inverted.  False or a zero will revert the display to normal/non-inverted.
void ssd1306::invert_screen(bool invert)
{
	send_command(OLED_INVERSE_DISABLE + invert);
}

// Draw a box defined by the co-ordinates of the top left corner plus the height/width dimensions.
// start_row:	0 to 63 starting at the top.
// start_col:	0 to 127 starting at the left.
// height:	1 to 64 pixels.
// width:	1 to 128 pixels.
void ssd1306::draw_box(uint8_t start_row, uint8_t start_column, uint8_t height, uint8_t width)
{
	uint8_t start_page = (start_row / 8);				// Addressing is done by (page, column), so calculate the first page from the start row.
	uint8_t num_pages = ((((start_row % 8) + height - 1) / 8) + 1);	// The number of pages spanned by the box.

	uint8_t empty_rows_in_top_page = (start_row % 8);								// For left-shifting top row segments.
	uint8_t empty_rows_in_bottom_page = (8 - (height - ((num_pages - 2) * 8) - (8 - empty_rows_in_top_page)));	// For right-shifting bottom row segments.

	set_address(start_page, start_column);	// Set address for the first segment.

	if(num_pages > 1)	// Boxes that span more than one page.
	{
		send_data(0xFF << empty_rows_in_top_page);					// Top left box segment.
		for(uint8_t c = (start_column + 1); c < (start_column + width - 1); c++)	// Cycle through columns for box top row.
		{
			send_data(0x01 << empty_rows_in_top_page);
		}
		send_data(0xFF << empty_rows_in_top_page);					// Top right box segment.
		for(uint8_t p = (start_page + 1); p < (start_page + num_pages - 1); p++)	// Cycle through pages between top and bottom page.
		{
			set_address(p, start_column);
			send_data(0xFF);							// Left side box segments.
			set_address(p, (start_column + width - 1));
			send_data(0xFF);							// Right side box segments.
		}
		set_address((start_page + num_pages - 1), start_column);
		send_data(0xFF >> empty_rows_in_bottom_page);					// Bottom left box segment.
		for(uint8_t c = (start_column + 1); c < (start_column + width - 1); c++)	// Cycle through columns for box bottom row.
		{
			send_data(0x80 >> empty_rows_in_bottom_page);
		}
		send_data(0xFF >> empty_rows_in_bottom_page);					// Bottom right box segment.
	}
	else	// Boxes that fit within a single page.
	{
		send_data((0xFF >> (8 - height)) << empty_rows_in_top_page);			// Left box segment.
		for(uint8_t c = (start_column + 1); c < (start_column + width - 1); c++)	// Cycle through for box top and bottom rows.
		{
			send_data(((0x01 << (height - 1)) + 1) << empty_rows_in_top_page);
		}
		send_data((0xFF >> (8 - height)) << empty_rows_in_top_page);			// Right box segment.
	}
}

// Print a test pattern to the display.
void ssd1306::test_pattern(void)
{
	send_command(OLED_SET_MEMORY_ADDRESSING_MODE, OLED_SET_MEMORY_ADDRESSING_MODE_HORIZONTAL);	// Temporarily set memory addressing mode to Horizontal Mode.
	send_command(OLED_ADDRESS_PAGE, 0, 7);								// Set page address range.
	send_command(OLED_ADDRESS_COLUMN, 0, 127);							// Set column address range.
	
	for (uint8_t i = 0; i < 64; i++)	// Can send packets of maximum 16 bytes at a time.  Therefore need to repeat 64 times for all segments.
	{
		Wire.beginTransmission(OLED_ADDR);
		Wire.write(OLED_CONTROL_BYTE_BULK_DATA);
		for (uint8_t j = 0; j < 16; j++)
		{
			Wire.write(0b11110000);		// Segment index increases every write.
		}
		Wire.endTransmission(OLED_ADDR);
		delay(20);				// Delay for dramatic effect.
	}
	send_command(OLED_SET_MEMORY_ADDRESSING_MODE, OLED_SET_MEMORY_ADDRESSING_MODE_DEFAULT);		// Reset memory addressing mode back to Page Mode.
}

// Take a character, obtain the character map data from the defined font, and print that character to the defined co-ordinates.
// Note, the string must be an array of unsigned characters.  Unsigned allows character integer values up to 255, instead of just 127.
// Values greater than 127 wanted fo this application due to the use of the degrees symbol '°' which has integer value 176.
// This is why passing strings in the format "string" requires typecasting to unsigned char: (unsigned char*)"string"
void ssd1306::print_char(unsigned char character, const uint8_t *font, uint8_t start_page, uint8_t start_column)
{
//	uint8_t font_width = pgm_read_byte(&&font[FONT_WIDTH]));		// From the font metadata obtain width of characters in pixels (this is max width for non-fixed-width fonts).
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
	uint16_t char_data_addr =	( (pgm_read_byte(&font[FIRST_CHAR_META + ((character - font_first_char) * 4) + CHAR_ADDR_MSB]) ) << 8	) +	// Character index MSB shifted left 8 bits +
					( (pgm_read_byte(&font[FIRST_CHAR_META + ((character - font_first_char) * 4) + CHAR_ADDR_LSB]) )	) +	// Character index LSB +
					( FIRST_CHAR_META + (font_num_chars * 4 ) );									// Number of bytes used by font and character metadata.

	// From the char metadata, obtain the number of data bytes used to define the char.
	uint8_t char_data_size = pgm_read_byte(&font[	FIRST_CHAR_META +			// Start at first character metadata byte.
							((character - font_first_char) * 4) +	// Skip to the metadatabytes for the current character.
							CHAR_BYTE_SIZE ]);			// Skip to the metadata byte that represents number of char data bytes.

	// From the char metadata, obtain the char width in pixels. 
	uint8_t char_width = pgm_read_byte(&font[	FIRST_CHAR_META +			// Start at first character metadata byte.
							((character - font_first_char) * 4) +	// Skip to the metadatabytes for the current character.
							CHAR_WIDTH ]);				// Skip to the metadata byte that represents char width.

	// Determine the minimum number of pages needed to display the full height of the character - pages are 8 pixels tall.
	uint8_t char_pages;
	if(font_height % 8)	char_pages = ((font_height + (8 - (font_height % 8))) / 8);	// Height in pixels is not a multiple of 8.
	else			char_pages = (font_height / 8);					// Height in pixels is a multiple of 8.

	// Run through each of the bytes of character data and send to the correct segment address.
	//	Page address : start_page + (b % char_pages)		: Starting from the top page, move down with each increment for the number of pages needed (char_pages), then roll back to the top page.
	//	Column address : start_column + (b / char_pages) 	: Starting from the left-most column, remain on that column for the required number of pages then move to the next column.
	for (uint8_t b = 0; b < char_data_size; b++)
	{
		send_segment(	pgm_read_byte(&font[char_data_addr]),	// Segment data.
				start_page + (b % char_pages),		// Page address.
				start_column + (b / char_pages) );	// Column address.

		char_data_addr++;	// Increment the working address to the next byte in the character data.
	}

	// Font data includes the minimum bytes needed without 0x00 bytes at the end.  Now need to send these 0x00 bytes to clear the remaining character frame.
	//	Number of these padding bytes calculated by minusing the number of character data bytes (char_data_size) from the total number of bytes needed.
	//	Total number of bytes needed calculated from the character width (char_width) multiplied by the number of pages needed for the character height (char_pages).
	for (uint8_t b = char_data_size; b < (char_pages * char_width); b++)
	{
		send_segment(	0x00,					// Segment data.
				start_page + (b % char_pages),		// Page address.
				start_column + (b / char_pages) );	// Column address.
	}
}

// Sequentially print each character in a string starting at the defined co-ordinates.
void ssd1306::print_string(unsigned char *string, const uint8_t *font, uint8_t start_page, uint8_t start_column)
{
	uint8_t font_first_char = pgm_read_byte(&font[FONT_FIRST_CHAR]);	// From the font metadata obtain the value of the first included character (often ' ' (i.e. space) or 0x20).
	uint8_t column = start_column;						// Initialise the working column - starts at start_column.

	uint8_t character = 0;		// Initialise the character index to be used in the character array.	
	while(string[character])	// Repeat the while loop until "character" is incremented to the end of the string.
	{
		// From the character metadata, determine width in pixels of the current char.
		uint8_t current_char_width = pgm_read_byte(&font[	FIRST_CHAR_META +				// Start at first character metadata byte.
									((string[character] - font_first_char) * 4) +	// Skip to the metadatabytes for the current character.
									CHAR_WIDTH ]);					// Skip to the metadata byte that represents character width.

		print_char(string[character], font, start_page, column);						// Print the current character to the oled.
		column += current_char_width;										// Increment the column index by the width of the character.
		character++;												// Increment to the next character in the string.
	}
}

// Print a full-resolution (128x64) image to screen.
void ssd1306::map_bits(const uint8_t *bitmap, const uint16_t bitmap_size)
{
	send_command(OLED_SET_MEMORY_ADDRESSING_MODE, OLED_SET_MEMORY_ADDRESSING_MODE_HORIZONTAL);	// Temporarily set memory addressing mode to Horizontal Mode.
	send_command(OLED_ADDRESS_PAGE, 0, 7);								// Set page address range.
	send_command(OLED_ADDRESS_COLUMN, 0, 127);							// Set column address range.

	uint16_t seg = 0;		// seg will increment for every segment sent to the OLED.
	while(seg < bitmap_size)	// Send every segment in the bitmap.
	{
		if(!(seg % 16))	// Can send bulk packets of maximum 16 bytes at a time.  Therefore need to restart every 16 transmissions.
		{
			Wire.endTransmission(OLED_ADDR);
			Wire.beginTransmission(OLED_ADDR);
			Wire.write(OLED_CONTROL_BYTE_BULK_DATA);
		}
		Wire.write(pgm_read_byte(&bitmap[seg]));	// Bitmap address increments 16 times in j loop, 64 times in i loop, total 1024 bytes.
		seg++;
	}
	while(seg < 1024)	// Bitmap may have had fewer than the total 1024 bytes so the rest should be zeros.
	{
		if(!(seg % 16))	// Can send bulk packets of maximum 16 bytes at a time.  Therefore need to restart every 16 transmissions.
		{
			Wire.endTransmission(OLED_ADDR);
			Wire.beginTransmission(OLED_ADDR);
			Wire.write(OLED_CONTROL_BYTE_BULK_DATA);
		}
		Wire.write(0x00);	// Just send zeros (blank segments).
		seg++;
	}
	Wire.endTransmission(OLED_ADDR);

	send_command(OLED_SET_MEMORY_ADDRESSING_MODE, OLED_SET_MEMORY_ADDRESSING_MODE_DEFAULT);		// Reset memory addressing mode back to Page Mode.
}
