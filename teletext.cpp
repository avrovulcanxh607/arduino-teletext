/*
 * teletext.cpp - Library for controlling STV5348 decoder and compatible ICs
 * Written by Nathan Dane in 2019
*/

#include "Arduino.h"
#include "Wire.h"
#include "teletext.h"

Teletext::Teletext(void)
{
	Wire.begin();
}

void Teletext::begin(void)
{
	writeByte(0x00,0x00); // 80
	writeByte(0x01,0x04);
	writeByte(0x02,0x00);
	writeByte(0x03,0x00);
	writeByte(0x04,0x00);
	writeByte(0x05,0xFF);
	writeByte(0x06,0xFF);
	writeByte(0x07,0x00); // 60 for cursor
	writeByte(0x08,0x10);

	callPage(1,0,0);  // Should probably read packet 8/30, but can't be bothered
}

bool Teletext::callPage(int M, int T, int U)
{
	if (M < 9 && M > 0)
	{
		setCursor(0x00,0x00);
		writeByte(0x0b,0x02); // green
		writeByte(0x0b,0x50); // P
		writeByte(0x0b,48+M); 
		writeByte(0x0b,48+T); // Write the Page status on row 0
		writeByte(0x0b,48+U);
		setCursor(0x00,0x02);
		
		if (M == 8)
		{
			M = 0;  // 8 becomes 0 because of binary stuff
		}
		
		writeByte(0x02,0x00);
		writeByte(0x03,24+M);
	
		writeByte(0x02,0x01);
		writeByte(0x03,16+T); // Write the page number to the register
	
		writeByte(0x02,0x02);
		writeByte(0x03,16+U);
		return true;
	}
	else  // If the value isn't valid, change the number red
	{
		setCursor(0x00,0x00);
		writeByte(0x0b,0x01); // red
		return false;
	}
	
}

bool Teletext::reveal(void)	// Toggle Reveal on/off, returns the set value
{
	if(!conceal)
	{
		writeByte(0x07,0x20); // Reveal
		conceal = true;
	}
	else
	{
		writeByte(0x07,0x00); // Conceal
		conceal = false;
	}
	return conceal;
}

bool Teletext::displayX24(void)
{
	if(!x24)
	{
		writeByte(0x00,0x80); // On
		x24 = true;
	}
	else
	{
		writeByte(0x00,0x00); // Off
		x24 = false;
	}
	return x24;
}

void Teletext::setCursor(int row, int column)
{
	writeByte(0x09,row); // row
	writeByte(0x0a,column); // column
}

void Teletext::writeByte(uint8_t subAddress, uint8_t data)
{
	Wire.beginTransmission(0x11);  // Initialize the Tx buffer
	Wire.write(subAddress);           // Put slave register address in Tx buffer
	Wire.write(data);                 // Put data in Tx buffer
	Wire.endTransmission();           // Send the Tx buffer
}
