#include <WiFlyHQ.h>
#include <SoftwareSerial.h>
#include "DHT.h"
#include <MD5.h>

#include <avr/io.h>
#include <avr/wdt.h>
#define Reset_AVR() wdt_enable(WDTO_30MS); while(1) {}

#define Light_PIN A1 //imposto il pin a cui è connesso il sensore
#define Smoke_PIN A2 //imposto il pin a cui è connesso il sensore
#define DHT22_PIN 2 //imposto il pin a cui è connesso il sensore

#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHT22_PIN, DHTTYPE);

#define Periodo_Invio_Dati 10000 //10s = tempo minimo tra un'invio sul web e l'altro.(ms)

char username[] = "root";  //username per login sito
char password[] = "";      //password per login sito

/*WIFI SETTING*/
WiFly wifly;

/* Change these to match your WiFi network */
const char mySSID[] = "DSG";
const char myPassword[] = "ABCD123456";
char* macString;

char serverName[] = "192.168.1.200";//URL del server a cui connettersi
#define serverPort 80 //porta di connessione
char pageName[] = "pagina_server.php";//nome pagina php per la ricezione dei dati

SoftwareSerial wifiSerial(8,9);

//VARIABILI PER I SENSORI
float light_accum = 0.0; 
float temp_accum = 0.0;
float smoke_accum = 0.0;
float hum_accum = 0.0;

unsigned long time = 0;
unsigned long SendTime = 0;

char lightBuffer[10];    
char smokeBuffer[10];    
char tempBuffer[10];    
char humBuffer[10];

char* secret="secret";
char jsonMsg[512];
char md5Input[512];
unsigned char* hash;
char *md5str = "5EB63BBBE01EEED093CB22BB8F5ACDC3";
char request[512];

void setup()
{
  // start the serial library:
  Serial.begin(9600);
  
  configWIFI();
  
  Serial.println("Connessione Configurata.");
  delay(1000);//aspetto un secondo per far avviare lo shield ethernet
  Serial.println("Programma Avviato, Setup Terminato!");

}


void loop()
{
    time = millis();
        
    if(time > SendTime + Periodo_Invio_Dati){
       SendTime = millis();
       light_accum = readLightSensor();
       smoke_accum = readSmokeSensor();
       readDHTSensor();              
       
       dtostrf(light_accum, 6, 2, lightBuffer);
       dtostrf(smoke_accum, 6, 2, smokeBuffer);
       dtostrf(temp_accum, 6, 2, tempBuffer);
       dtostrf(hum_accum, 6, 2, humBuffer);
             
       sprintf(md5Input, "%s%s", macString, secret);
            
       //hash=MD5::make_hash(md5Input);
       //md5str = MD5::make_digest(hash,16);
              
       Serial.println("CREO JSON");
       sprintf(jsonMsg, "{\"checksum\":\"%s\",\"mac\":\"%s\",\"l\":%s,\"s\":%s,\"t\":%s,\"h\":%s}", md5str, macString, lightBuffer, smokeBuffer, tempBuffer, humBuffer);
             
       Serial.println("connessione...");             
       Serial.println("invio dati sensori");
       Serial.println(jsonMsg);
       //InvioWIFIHttp(serverName,serverPort,pageName,username,password,JSONString);
       Serial.println("fine invio");
                      
       Reset_AVR();       
    }   
}

float readSmokeSensor(){
    int val = analogRead(Smoke_PIN);     // read the value from the analog sensor
    float Ro = 12000.0;    // this has to be tuned 12K Ohm
    
    float Vrl = val * ( 5.00 / 1024.0  );      // V
    float Rs = 20000 * ( 5.00 - Vrl) / Vrl ;   // Ohm 
    float ratio =  Rs/Ro;    
    
    float ppm = 0.0;
    ppm = 37143 * pow (ratio, -3.178);
    return ppm;    
}

float readLightSensor(){
   float luce = analogRead(Light_PIN); // Inserisco il valore della lettura dell'input analogico sull'intero ValoreSensore
   luce = (5.0* luce * 200.0) / 1024.0;   
   return luce;
}

void readDHTSensor(){
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  
  // check if returns are valid, if they are NaN (not a number) then something went wrong!
  if ((isnan(t))||(isnan(h))) {
    Serial.println("Failed to read from DHT");
  } else {
    temp_accum=t;
    hum_accum=h;
   }  
}

void configWIFI(){
    Serial.println("Starting");
    Serial.print("Free memory: ");
    Serial.println(wifly.getFreeMemory(),DEC);

    wifiSerial.begin(9600);
    if (!wifly.begin(&wifiSerial, &Serial)) {
        Serial.println("Failed to start wifly");
	Reset_AVR();
    }

    char buf[32];
    
    if (!wifly.isAssociated()) {
	Serial.println("Joining network");
	wifly.setSSID(mySSID);
	wifly.setPassphrase(myPassword);
	wifly.enableDHCP();

	if (wifly.join()) {
	    Serial.println("Joined wifi network");
	} else {
	    Serial.println("Failed to join wifi network");
	    Reset_AVR();
	}
    } else {
        Serial.println("Already joined network");
    }

    Serial.print("MAC: ");
    Serial.println(wifly.getMAC(buf, sizeof(buf)));
    wifly.setDeviceID("Wifly-WebClient");
    
    if (wifly.isConnected()) {
        Serial.println("Old connection active. Closing");
	wifly.close();
    }
    
    if (wifly.open(serverName, 80)) {
        Serial.print("Connected to ");
	Serial.println(serverName);

	Serial.println("WIFI ALREADY");
    } else {
        Serial.println("Failed to connect");
        Reset_AVR();
    }   
}

void InvioWIFIHttp(char server[], int porta, char pagina[], char username[], char* jsonString)
{
  wifly.print("GET /arduino/");
  wifly.print(pagina);
  wifly.print("?username=");
  wifly.print(username);
  wifly.print("&password=");
  wifly.print(password);
  wifly.print("&JSON=");
  wifly.print(jsonString);
}

