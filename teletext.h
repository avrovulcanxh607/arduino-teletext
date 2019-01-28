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
		void writeByte(uint8_t,uint8_t);
	private:
		bool conceal = false;
		bool x24 = false;
};

#endif
