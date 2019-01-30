/*
 * teletext.h - Library for controlling STV5348 decoder and compatible ICs
 * Written by Nathan Dane in 2019
*/

#ifndef teletext_h
#define teletext_h

#include "Arduino.h"

class Teletext
{
	public:
		Teletext(void);
		void begin(void);
		bool callPage(int,int,int);
		bool reveal(void);
		bool displayX24(void);
		void setCursor(int,int);
		void writeChar(char);
		bool dispCursor(void);
		void doubleSize(int);
		void fastText(int);
		void writeByte(uint8_t,uint8_t);
		int readByte(uint8_t);
	private:
		byte deHam(uint8_t);
		bool conceal = false;
		bool cursor = false;
		bool x24 = false;
		uint8_t reg00 = 0;
		uint8_t reg01 = 16;
		uint8_t reg02 = 0;
		uint8_t reg03 = 0;
		uint8_t reg04 = 0;
		uint8_t reg05 = 255;
		uint8_t reg06 = 255;
		uint8_t reg07 = 0;
		uint8_t reg08 = 16;
};

#endif
