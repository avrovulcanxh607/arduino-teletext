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
      teletext.writeChar(receivedChar);
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
    else if (receivedChar == 'a')
    {
      bool cursorresult=teletext.dispCursor();
      if (cursorresult)
      {
        Serial.println("Cursor On");
      }
      else
      {
        Serial.println("Cursor Off");
      }
    }
    else if (receivedChar == 't')
    {
      Serial.println("Top");
      teletext.doubleSize(1);
    }
    else if (receivedChar == 'b')
    {
      Serial.println("Bottom");
      teletext.doubleSize(2);
    }
    else if (receivedChar == 'n')
    {
      Serial.println("Normal");
      teletext.doubleSize(0);
    }
    else if (receivedChar == 'f')
    {
      Serial.println("Line 24 Toggle");
      teletext.displayX24();
    }
    else if (receivedChar == 'r')
    {
      Serial.println("Fasttext Red");
      teletext.fastText(0);
    }
    else if (receivedChar == 'g')
    {
      Serial.println("Fasttext Green");
      teletext.fastText(1);
    }
    else if (receivedChar == 'y')
    {
      Serial.println("Fasttext Yellow");
      teletext.fastText(2);
    }
    else if (receivedChar == 'u')
    {
      Serial.println("Fasttext Cyan");
      teletext.fastText(3);
    }
    else if (receivedChar == 'h')
    {
      Serial.println("Fasttext Link 5");
      teletext.fastText(4);
    }
    else if (receivedChar == 'x')
    {
      teletext.writeByte(0x04,0x04);
    }
    else if (receivedChar == 'z')
    {
      teletext.writeByte(0x04,0x00);
    }
  }
}
