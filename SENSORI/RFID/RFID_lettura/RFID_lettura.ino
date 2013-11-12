/*
  works with 13.56MHz MiFare 1k tags

  Based on hardware v13:
  D7 -> RFID RX
  D8 -> RFID TX
    
  Note: RFID Reset attached to D13 (aka status LED)
  
  Note: be sure include the NewSoftSerial lib, http://arduiniana.org/libraries/newsoftserial/
  
  Usage: Sketch prints 'Start' and waits for a tag. When a tag is in range, the shield reads the tag,
  blinks the 'Found' LED and prints the serial number of the tag to the serial port
  and the XBee port. 

*/
#include <SoftwareSerial.h>

SoftwareSerial rfid(7, 8);

//Prototypes
void halt(void);		//Procedura di arresto	
void parse(void);		//Procedura che legge la stringa dal modulo
void print_serial(void);	//Procedura che stamp ail numero seriale
void read_serial(void);	        //Procedura che legge dal modulo
void seek(void);		//commando per mettersi in attesa del tag
void set_flag(void);		//imposto flag se ho ricevuto un tag corretto

//Global var
int flag = 0;			//FLAG per il tag
int Str1[11];			//Buffer di ricezione

//Led Riconoscimento
int ledDigital[3] = {3, 5, 6}; 

const boolean ON = LOW;   //Define on as LOW (this is because we use a common Anode RGB LED (common pin is connected to +5 volts)
const boolean OFF = HIGH; //Define off as HIGH

//Predefined Colors
const boolean RED[] = {ON, OFF, OFF};    
const boolean GREEN[] = {OFF, ON, OFF}; 
const boolean BLUE[] = {OFF, OFF, ON}; 
const boolean YELLOW[] = {ON, ON, OFF}; 
const boolean CYAN[] = {OFF, ON, ON}; 
const boolean MAGENTA[] = {ON, OFF, ON}; 
const boolean WHITE[] = {ON, ON, ON}; 
const boolean BLACK[] = {OFF, OFF, OFF}; 

//An Array that stores the predefined colors (allows us to later randomly display a color)
const boolean* COLORS[] = {RED, GREEN, BLUE, YELLOW, CYAN, MAGENTA, WHITE, BLACK};

//INIT
void setup()  
{
  for(int i = 0; i < 3; i++){   
    pinMode(ledDigital[i], OUTPUT);   //Set the three LED pins as outputs   
  }
  
  Serial.begin(9600);
  Serial.println("Start");
  
  // set the data rate for the SoftwareSerial ports
  rfid.begin(19200);
  delay(10);
  halt();
}

//MAIN
void loop()                 
{
  read_serial();
}

void halt()
{
 //Halt tag
  rfid.write((byte)0xFF);
  rfid.write((byte)0x00);
  rfid.write((byte)0x01);
  rfid.write((byte)0x93);
  rfid.write((byte)0x94);
}

void parse()
{
  while(rfid.available()){
    if(rfid.read() == 255){
      for(int i=1;i<11;i++){
        Str1[i]= rfid.read();
      }
    }
  }
}

void print_serial()
{
  if(flag == 1){
    riconosci();
    //print to serial port
    Serial.print(Str1[8], HEX);
    Serial.print(Str1[7], HEX);
    Serial.print(Str1[6], HEX);
    Serial.print(Str1[5], HEX);
    Serial.println();
    delay(100);    
  }
}

void riconosci(){
  if((Str1[8])==176){              //TAG RJ
      setColor(ledDigital, RED);
      delay(100);    
      setColor(ledDigital, BLACK);
      delay(100);    
      setColor(ledDigital, RED);
      delay(100);         
  }
  
  if((Str1[8])==165){               //TAG H
      setColor(ledDigital, BLUE);
      delay(100);    
      setColor(ledDigital, BLACK);
      delay(100);    
      setColor(ledDigital, BLUE);
      delay(100);    
  }
  
}

void read_serial()
{
  seek();
  delay(10);
  parse();
  set_flag();
  print_serial();
  delay(100);
  setColor(ledDigital, BLACK);
}

void seek()
{
  //search for RFID tag
  rfid.write((byte)0xFF);
  rfid.write((byte)0x00);
  rfid.write((byte)0x01);
  rfid.write((byte)0x82);
  rfid.write((byte)0x83); 
  delay(10);
}

void set_flag()
{
  if(Str1[2] == 6){
    flag++;
  }
  if(Str1[2] == 2){
    flag = 0;
  }
}

//PROCEDURE PER SETTARE IL COLORE DEL LED
void setColor(int* led, boolean* color){ 
  for(int i = 0; i < 3; i++){   
    digitalWrite(led[i], color[i]); 
  }
}

void setColor(int* led, const boolean* color){  
  boolean tempColor[] = {color[0], color[1], color[2]};  
  setColor(led, tempColor);
}

