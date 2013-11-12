#include <SoftwareSerial.h>
#include "DHT.h"
#include <SPI.h>
#include <Ethernet.h>
#include <string.h>

#define interval 5000  //Intervallo di tempo per la lettura del DHT11 

long previousMills = 0;  //Memorizza l'ultima volta che ho letto un dato

#define DHTPIN 8     // what pin we're connected to
#define DHTTYPE DHT11   // DHT 22  (AM2302)

DHT dht(DHTPIN, DHTTYPE);

SoftwareSerial lcd(2, 3);
const int buttonPinLCD = 4;
const int buttonPinBuzzer = 5;

const int Buzzer = 7;

int page_counter = 0;
int old_page_counter = 0;
bool premuto = false;
bool old_premuto = true;
bool toWrite = false;

int humValue;
int old_hum = 0;
int tempValue;
int old_temp = 0;

// MAC address can be anything that is unique within your network.
// IP is the address the Arduino Ethernet Card would respond to.  It needs to be an unused address within your network.
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0x1E, 0x93 };
byte ip[] = { 192, 168, 0, 205 };

EthernetServer server(5000); // Port 5000 is http.

void setup(){
  Serial.begin(9600);
  
  lcd.begin(9600);
  
  pinMode(buttonPinLCD, INPUT);
  pinMode(buttonPinBuzzer, INPUT);

  pinMode(Buzzer, OUTPUT);
  digitalWrite(Buzzer, LOW);
  
  dht.begin(); 
  
  Serial.println("...Configuro la connessione ethernet...");
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("My IP address: ");
  Serial.println(Ethernet.localIP());

  Serial.println("Ready");  
}

void loop(){
  EthernetClient client = server.available();
  if (client)
  {
    Serial.println("REQUEST INCOMING..");
    WaitForRequest(client);
    ParseReceivedRequest();
    PerformRequestedCommands();

    Serial.println("Richiesta servita");
    byte a;
    while (client.available()) {a=client.read();}
    client.stop();
    EmptyBuffer();    
  }
  
  if(digitalRead(buttonPinLCD)==0){ 
    //delay(10); //ANTI_RIMBALZO    

    if(page_counter < 3) page_counter++;
    else page_counter = 0;

    while(digitalRead(buttonPinLCD)==0);
  }  
  
  digitalWrite(Buzzer, digitalRead(buttonPinBuzzer)-1);
  
  printOnLCD(page_counter);
  
  if (millis() -  previousMills > interval ){
         previousMills = millis();  //Tiene in memoria l'ultimo invio
         readDHT11Sensor();
  }  
}

void EmptyBuffer(){
  byte a;
  while (Serial.available()) {a=Serial.read();}
}




