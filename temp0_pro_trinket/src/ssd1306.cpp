#include "ssd1306.h"

ssd1306::ssd1306(void)
{
	Wire.begin();	//Communicating with the ssd1306 requires I2C.
}

void ssd1306::send_command(uint8_t command)
{
	Wire.beginTransmission(OLED_ADDR);
	Wire.write(OLED_CONTROL_BYTE_COMMAND);
	Wire.write(command);
	Wire.endTransmission(OLED_ADDR);
}

void ssd1306::send_command(uint8_t command, uint8_t value)
{
	Wire.beginTransmission(OLED_ADDR);
	Wire.write(OLED_CONTROL_BYTE_COMMAND);
	Wire.write(command);
	Wire.write(OLED_CONTROL_BYTE_COMMAND);
	Wire.write(value);
	Wire.endTransmission(OLED_ADDR);
}

//Send a data byute - i.e. 8-bits to set/clear the 8 pixels at the current address.
void ssd1306::send_data(uint8_t data)
{
	Wire.beginTransmission(OLED_ADDR);
	Wire.write(OLED_CONTROL_BYTE_DATA);
	Wire.write(data);
	Wire.endTransmission(OLED_ADDR);
}

//In page addressing mode, valid pages are 0 to 7 (top to bottom).
void ssd1306::set_page(uint8_t page)
{
	send_command(OLED_ADDRESS_FIRST_PAGE + page);
}

//In page addressing mode, valid pages are 0 to 127 (left to right).
void ssd1306::set_column(uint8_t column)
{
	send_command(OLED_ADDRESS_FIRST_COLUMN_LOWER | (column & 0b1111), OLED_ADDRESS_FIRST_COLUMN_HIGHER | ((column >> 4)));
}

//Set the page then column address to be set by the next data write.
//Note, each data write increments the column automatically, but does not change the page.
void ssd1306::set_address(uint8_t page, uint8_t column)
{
	set_page(page);
	set_column(column);
}

void ssd1306::clear_screen(void)
{
	for (uint8_t page=0; page<8; page++)		//Cycle through all 8 pages.
	{
		set_address(page, 0);			//Column starts at zero and increments with every data write.
		for (uint8_t col=0; col<128; col++)	//Cycle through all 128 columns.
		{
			send_data(0x00);		//Send all zeros to clear.
		}
	}
}

void ssd1306::draw_box(uint8_t start_page, uint8_t start_column, uint8_t height, uint8_t width)
{
	set_address(start_page, start_column);

	send_data(0xFF);							//Left most column, first page. 

	for(uint8_t c = start_column; c < (start_column+width-2); c++)		//Run through top row first page for all columns.
	{
		if(height>1)	{send_data(0b00000001);}			//Top row only.
		else		{send_data(0b10000001);}			//Top and bottom fows.
	}

	send_data(0xFF);							//Right most column, first page.

	if(height>1)								//More than a single page.
	{
		for(uint8_t p = (start_page+1); p < (start_page + height); p++)	//Run through left- and right-most column for all pages between first and last.
		{
			set_address(p, start_column);				//Left column.
			send_data(0xFF);
			set_address(p, (start_column+width-1));			//Right column.
			send_data(0xFF);
		}
		set_address((start_page+height-1), (start_column));		//Run through bottom row last page for all columns.
		send_data(0xFF);
		for(uint8_t c = start_column; c < (start_column+width-2); c++)
		{
			send_data(0b10000000);					//Bottom row only.
		}
		send_data(0xFF);						//Right most column, last page. 
	}
}

void ssd1306::init(void)
{
	send_command(OLED_SET_MULTIPLEX_RATIO, OLED_DEFAULT_MULTIPLEX_RATIO);			//Set MUX ratio.
	send_command(OLED_SET_DISPLAY_OFFSET, OLED_DEFAULT_DISPLAY_OFFSET);			//Set display offset.
	send_command(OLED_SET_DISPLAY_START_LINE);						//Set display start line.
	send_command(OLED_SET_SEGMENT_REMAP_COL0);						//Set segment re-map.
	send_command(OLED_SET_COM_OUTPUT_SCAN_DIR_NORMAL);					//Set COM output scan direction.
	send_command(OLED_SET_COM_PINS_HARDWARE_CONFIG, OLED_DEFAULT_COM_PINS_HARDWARE_CONFIG);	//Set COM pins hardware configuration.
	send_command(OLED_CONTRAST, OLED_DEFAULT_CONTRAST);					//Set contrast control.
	send_command(OLED_ALL_ON_RESUME);							//Disable entire display on.
	send_command(OLED_INVERSE_DISABLE);							//Set Normal display (i.e. not inverse).
	send_command(OLED_SET_DISPLAY_CLOCK, OLED_DEFAULT_DISPLAY_CLOCK);			//Set Osc frequency.
	send_command(OLED_SET_PRECHARGE_PERIOD, OLED_DEFAULT_PRECHARGE_PERIOD);			//Enable charge pump regulator.
	send_command(OLED_SET_MEMORY_ADDRESSING_MODE, OLED_DEFAULT_MEMORY_ADDRESSING_MODE);	//Set memory addressing mode to Page Addressing Mode.
	clear_screen();										//Clear any remnant screen memory from last run.
	send_command(OLED_ON);									//Turn display on.
}

void ssd1306::print_large_char(unsigned char character, uint8_t start_page, uint8_t start_column)
{
	for(uint8_t p=0; p<2; p++)							//Characters span two pages.
	{
		set_address((start_page+p), start_column);
		for(uint8_t i=(0+(8*p)); i<(8+(8*p)); i++)				//Cycle through each of the 8 segments on the current character page.
		{
			send_data(pgm_read_byte(&(font_8x16[character-32][i])));	//Send the data that corresponds to the segments defined in the font.
		}									//Note "-32" is required to map the decimal value of <char> to the defined font.
	}
}

void ssd1306::print_large_string(unsigned char *string, uint8_t start_page, uint8_t start_column)
{
	uint8_t c = 0;
	while(string[c])
	{
		print_large_char(string[c], start_page, (start_column+(c*8)));
		c++;
	} 
}

void ssd1306::print_large_double(double number, uint8_t start_page, uint8_t start_column)
{
	char buffer[5];
	dtostrf(number, 4, 2, buffer);	//Double-to-String function.  Syntax dtostrf(double, min_string_length, num_post_decimal_values, char_array_addr).
	print_large_string((unsigned char *) buffer, start_page, start_column);
}

void ssd1306::test_pattern(void)
{
	for (uint8_t page=0;page<8;page++)
	{
		send_command(page);
		for (uint8_t col=0;col<128;col++)
		{
			send_command((col&0b1111), ((col>>4)+0b10000));
			set_address(page, col);
			send_data(0b00001111);
		}
	}
}