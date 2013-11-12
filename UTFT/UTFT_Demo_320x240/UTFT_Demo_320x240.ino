#include <UTFT.h>
#include <SD.h>

// Declare which fonts we will be using
extern uint8_t SmallFont[];

// Uncomment the next line for Arduino 2009/Uno
//UTFT(byte model, int RS, int WR,int CS,int RD)
UTFT myGLCD(ITDB32S,A1,A2,A0,A3);    // Remember to change the model parameter to suit your display module!


void setup()
{
  randomSeed(analogRead(0));
  
// Setup the LCD
  myGLCD.InitLCD();
  myGLCD.setFont(SmallFont);

  myGLCD.setColor(255, 0, 0);
  myGLCD.fillRect(0, 0, 319, 13);
  myGLCD.setColor(64, 64, 64);
  myGLCD.fillRect(0, 226, 319, 239);
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(255, 0, 0);
  myGLCD.print("* MANCIO90 *", CENTER, 1);
  myGLCD.setBackColor(64, 64, 64);
  myGLCD.setColor(255,255,0);
  myGLCD.print("www.mancio90.it", CENTER, 227);
}

void loop()
{
  int buf[318];
  int x, x2;
  int y, y2;
  int r;

// Clear the screen and draw the frame
  //myGLCD.clrScr();

  myGLCD.setColor(0, 0, 255);
  myGLCD.drawRect(0, 14, 319, 225);

  myGLCD.setColor(255, 0, 0);
  myGLCD.fillRoundRect(80, 70, 239, 100);
  
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(255, 0, 0);
  myGLCD.print("NON SPEGNERE!", CENTER, 90);
  
  myGLCD.setColor(0, 255, 0);
  myGLCD.setBackColor(0, 0, 255);
  myGLCD.print("Runtime: (msecs)", CENTER, 150);
  myGLCD.printNumI(millis(), CENTER, 180);
  
  delay (1000);
}

