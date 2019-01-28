#include <teletext.h>
#include <Wire.h>

Teletext teletext;

int i = 1;
int Mag, Pten, Punit;

void setup() {
  Serial.begin(9600);
  Serial.println("Teletext In-Vision Decoder\r\n");
  teletext.begin();
}

void loop() {
  if (Serial.available()){
    char receivedChar = Serial.read();
    if (isDigit(receivedChar))
    {
      if (i == 1)
      {
        teletext.setCursor(0x00,0x02);
        teletext.writeByte(0x0b,0x20);
        teletext.writeByte(0x0b,0x20); // Clear the status line
        teletext.writeByte(0x0b,0x20);
        teletext.setCursor(0x00,0x02);
      }
      teletext.writeByte(0x0b,receivedChar);
      Serial.print(receivedChar);
      if (i == 1)
      {
        Mag = (int)receivedChar;
      }
      if (i == 2)
      {
        Pten = (int)receivedChar;
      }
      if (i == 3)
      {
        Punit = (int)receivedChar;
        bool callresult=teletext.callPage(Mag-48,Pten-48,Punit-48);
        if (callresult)
        {
          Serial.println(" Valid");
        }
        else
        {
          Serial.println(" Invalid");
        }
      }
  
      i++;
      if (i > 3)
      {
        i = 1;
      }
    }
    else if (receivedChar == 'c')
    {
      bool revealresult=teletext.reveal();
      if (revealresult)
      {
        Serial.println("Reveal");
      }
      else
      {
        Serial.println("Conceal");
      }
    }
  }
}
