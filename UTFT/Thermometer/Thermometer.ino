#include <UTFT.h>
#include <SD.h>

// Declare which fonts we will be using
extern uint8_t SmallFont[];

// Uncomment the next line for Arduino 2009/Uno
//UTFT(byte model, int RS, int WR,int CS,int RD)
UTFT myGLCD(ITDB32S,A1,A2,A0,A3);    // Remember to change the model parameter to suit your display module!
int delta = 100;
int delta2 = 0;
int inv = -1, inv2 = 1;

void setup()
{
  randomSeed(analogRead(0));
  
// Setup the LCD
  myGLCD.InitLCD();
  myGLCD.setFont(SmallFont);

  //BARRA DI STATO IN ALTO
  myGLCD.setColor(255, 0, 0);
  myGLCD.fillRect(0, 0, 319, 13);
  
  //BARRA IN BASSO
  myGLCD.setColor(64, 64, 64);
  myGLCD.fillRect(0, 226, 319, 239);
  
  //COLORE SCRITTE
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(255, 0, 0);
  myGLCD.print("* TEMPERATURA *", CENTER, 1);
  
  myGLCD.setBackColor(64, 64, 64);
  myGLCD.setColor(255,255,0);
  myGLCD.print("www.mancio90.it", CENTER, 227);
  
  //BORDO TERMOMETRO
  myGLCD.setColor(0, 0, 255);
  myGLCD.drawCircle(50, 180, 31);
  //myGLCD.drawCircle(50, 50, 15);
  myGLCD.drawRect(35, 50, 65, 180);
  
  myGLCD.print("100", 70, 50);
  myGLCD.print("0", 70, 135);
  
}

void loop()
{
  // Clear the screen and draw the frame
  //myGLCD.clrScr();

  /*myGLCD.setColor(0, 0, 255);
  myGLCD.drawRect(0, 14, 319, 225);

  myGLCD.setColor(255, 0, 0);
  myGLCD.fillRoundRect(80, 70, 239, 100);
  
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(255, 0, 0);
  myGLCD.print("NON SPEGNERE!", CENTER, 90);
  
  myGLCD.setColor(0, 255, 0);
  myGLCD.setBackColor(0, 0, 255);
  myGLCD.print("Runtime: (msecs)", CENTER, 150);
  myGLCD.printNumI(millis(), CENTER, 180);*/
    
  myGLCD.setColor(255, 0, 0);
  myGLCD.fillCircle(50, 180, 30);
  myGLCD.fillRect(35+1, 50+1+delta, 65-1, 180-1);
  myGLCD.setColor(0, 0, 0);
  myGLCD.fillRect(35+1, 50+1, 65-1, 150+1-delta2);
  
  char buffer[80];
  sprintf(buffer, "CI SONO %d GRADI!", delta2);
  myGLCD.print(buffer, CENTER, 100);
  
  delta += inv*10;
  delta2 += inv2*10;
  if( (delta<=0)||(delta>=100) ){ 
    inv *= -1;
  }
  if( (delta2<=0)||(delta2>=100) ){ 
    inv2 *= -1;
  }
  
  delay (1000);
}

