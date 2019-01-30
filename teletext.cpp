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
	writeByte(0x00,reg00);
	writeByte(0x01,reg01);
	writeByte(0x02,reg02);
	writeByte(0x03,reg03);
	writeByte(0x04,reg04);
	writeByte(0x05,reg05);
	writeByte(0x06,reg06);
	writeByte(0x07,reg07);
	writeByte(0x08,reg08);

	callPage(1,0,0);  // Should probably read packet 8/30, but can't be bothered
}

bool Teletext::callPage(int M, int T, int U)	// Calls a Page
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
		bitSet(reg07,5);
		writeByte(0x07,reg07); // Reveal
		conceal = true;
	}
	else
	{
		bitClear(reg07,5);
		writeByte(0x07,reg07); // Conceal
		conceal = false;
	}
	return conceal;
}

bool Teletext::displayX24(void)	// Toggle Line 24 On/off, returns the set value
{
	if(!x24)
	{
		bitSet(reg00,7);
		writeByte(0x00,reg00); // On
		x24 = true;
	}
	else
	{
		bitClear(reg00,7);
		writeByte(0x00,reg00); // Off
		setCursor(0x18,0x00);
		for(int i=0; i<40; ++i)	// Delete Line 24 when disabled
		{
			writeChar(0x20);
		}
		x24 = false;
	}
	return x24;
}

bool Teletext::dispCursor(void)	// Toggle Cursor on/off, returns the set value
{
	if(!cursor)
	{
		bitSet(reg07,6);
		writeByte(0x07,reg07); // Reveal
		cursor = true;
	}
	else
	{
		bitClear(reg07,6);
		writeByte(0x07,reg07); // Conceal
		cursor = false;
	}
	return cursor;
}

void Teletext::doubleSize(int half)	// Double Height Page
{
	if(half == 0)
	{
		bitClear(reg07,3);
		bitClear(reg07,4);
		writeByte(0x07,reg07); // Cancel
	}
	else if(half == 1)
	{
		bitSet(reg07,3);
		bitClear(reg07,4);
		writeByte(0x07,reg07); // Top Half
	}
	else if(half == 2)
	{
		bitSet(reg07,3);
		bitSet(reg07,4);
		writeByte(0x07,reg07); // Bottom Half
	}
}

void Teletext::setCursor(int row, int column)
{
	writeByte(0x09,row); // row
	writeByte(0x0a,column); // column
}

void Teletext::writeChar(char input)	// Write a Character to the Framebuffer
{
	bitClear(reg00,0);	// Make sure we're writing to 11A not 11B
	writeByte(0x00,reg00);
	writeByte(0x0b,input);
}

void Teletext::fastText(int link)
{
	int offset = link*6;
	
	bitSet(reg01,4);	// Turn on Ghost Row Mode
	writeByte(0x01,reg01);
	bitClear(reg00,0);	// Make sure we're writing to 11A not 11B
	writeByte(0x00,reg00);
	bitSet(reg08,2);	// Chapter 4 Active
	writeByte(0x08,reg08);
	
	setCursor(0x10,0x01+offset);
	byte read = readByte(0x0b);	// Read Page Unit
	int Unit = deHam(read);
	
	setCursor(0x10,0x02+offset);
	read = readByte(0x0b);
	int Ten = deHam(read);	// Read Page Ten
	
	setCursor(0x10,0x04+offset);
	read = readByte(0x0b);
	int byte4 = deHam(read);
	setCursor(0x10,0x06+offset);	// Read page Mag
	read = readByte(0x0b);
	int byte6 = deHam(read);
	
	int M1 = bitRead(byte4,3);
	int M2 = bitRead(byte6,3);
	int M3 = bitRead(byte6,2);
	int mag=0;
	if (M1 == 1)
		bitSet(mag,0);
	if (M2 == 1)
		bitSet(mag,2);	// Work out actual Mag
	if (M3 == 1)
		bitSet(mag,1);
	
	bitClear(reg08,2);	// Chapter 0 Active
	writeByte(0x08,reg08);
	setCursor(0x19,0x08);
	int curmag = readByte(0x0b);	// Read current Mag
	
	int Mag = curmag ^ mag;
	
	callPage(Mag,Ten,Unit);
}

void Teletext::writeByte(uint8_t subAddress, uint8_t data)	// Write a Byte to the I2C IC
{
	Wire.beginTransmission(0x11);  // Initialize the Tx buffer
	Wire.write(subAddress);           // Put slave register address in Tx buffer
	Wire.write(data);                 // Put data in Tx buffer
	Wire.endTransmission();           // Send the Tx buffer
}

int Teletext::readByte(uint8_t subAddress)	// Read a Byte from the I2C IC
{
	Wire.beginTransmission(0x11);    // Get the slave's attention, tell it we're sending a command byte
	Wire.write(subAddress);                               //  The command byte, sets pointer to register with address of 0x32
	Wire.requestFrom(0x11,1);          // Tell slave we need to read 1byte from the current register
	int slaveByte2 = Wire.read();        // read that byte into 'slaveByte2' variable
	Wire.endTransmission();
	return slaveByte2;
}

byte Teletext::deHam(uint8_t input)	// This is REALLY terrible and should never be repeated. Shhh...
{
	byte out=0;
	
	int bit1 = bitRead(input,7);
	int bit2 = bitRead(input,5);
	int bit3 = bitRead(input,3);
	int bit4 = bitRead(input,1);
	
	if (bit1 == 1)
		bitSet(out,3);
	if (bit2 == 1)
		bitSet(out,2);
	if (bit3 == 1)
		bitSet(out,1);
	if (bit4 == 1)
		bitSet(out,0);
	
	return out;
}