#include <SPI.h>
#include <WiFlyHQ.h>
#include <SoftwareSerial.h>
#include "DHT.h"

#include <avr/io.h>
#include <avr/wdt.h>
#define Reset_AVR() wdt_enable(WDTO_30MS); while(1) {}

#define Light_PIN A1 //imposto il pin a cui è connesso il sensore
#define Smoke_PIN A2 //imposto il pin a cui è connesso il sensore
#define DHT22_PIN 2 //imposto il pin a cui è connesso il sensore

#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHT22_PIN, DHTTYPE);

#define ledLight_PIN 3
#define ledSmoke_PIN 5
#define ledTemp_PIN 6
#define ledHum_PIN 7

#define Periodo_Invio_Dati 10000 //10s = tempo minimo tra un'invio sul web e l'altro.(ms)
#define Periodo_Lettura_Sensore 2000 //2s = tempo minimo tra una lettura del sensore e l'altra (ms)

#define id_Light 2000
#define id_Smoke 3000
#define id_Temp 4000
#define id_Hum 5000

char username[] = "root";  //username per login sito
char password[] = "";      //password per login sito

/*WIFI SETTING*/
WiFly wifly;

/* Change these to match your WiFi network */
const char mySSID[] = "DSG";
const char myPassword[] = "ABCD123456";
char* macString = "00:11:00:11:00:11";
//const char site[] = "192.168.1.2";

void terminal();
void print_P(const prog_char *str);
void println_P(const prog_char *str);

char serverName[] = "192.168.1.200";//URL del server a cui connettersi
#define serverPort 80 //porta di connessione
char pageName[] = "pagina_server.php";//nome pagina php per la ricezione dei dati

SoftwareSerial wifiSerial(8,9);

int light_accum = 0; 
float temp_accum = 0.0;
float smoke_accum = 0.0;
float hum_accum = 0.0;

long n_camp = 0;

float avg_light = 0.0;
float avg_smoke = 0.0;
float avg_temp = 0.0;
float avg_hum = 0.0;

unsigned long time = 0;
unsigned long SendTime = 0;
unsigned long ReadTime = 0;

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
  
  pinMode(ledLight_PIN, OUTPUT);
  pinMode(ledSmoke_PIN, OUTPUT);
  pinMode(ledTemp_PIN, OUTPUT);
  pinMode(ledHum_PIN,OUTPUT);
  Serial.println("Connessione Configurata.");
  delay(1000);//aspetto un secondo per far avviare lo shield ethernet
  Serial.println("Programma Avviato, Setup Terminato!");

}


void loop()
{
    time = millis();
        
    if(time > SendTime + Periodo_Invio_Dati){
       SendTime = millis();
       avg_light = float(light_accum / double(n_camp));//calcolo la media delle lettura
       avg_smoke = float(smoke_accum / double(n_camp));//calcolo la media delle lettura
       avg_temp = float(temp_accum / double(n_camp));//calcolo la media delle lettura
       avg_hum = float(hum_accum / double(n_camp));
       if(n_camp > 0){
         Serial.println("connessione...");
                
             dtostrf(avg_light, 6, 2, lightBuffer);
             dtostrf(avg_smoke, 6, 2, smokeBuffer);
             dtostrf(avg_temp, 6, 2, tempBuffer);
             dtostrf(avg_hum, 6, 2, humBuffer); 
             
             Serial.println("invio lettura sensori");
             Serial.println("CREO JSON");
             sprintf(jsonMsg, "{\"checksum\":\"%s\",\"mac\":\"%s\",\"l\":%s,\"s\":%s,\"t\":%s,\"h\":%s}", md5str, macString, lightBuffer, smokeBuffer, tempBuffer, humBuffer);
             Serial.println(jsonMsg);
             delay(10000);
             //InvioWIFIHttp(serverName,serverPort,pageName,username,password,id_Light,avg_light);
             Serial.println("fine invio");

             digitalWrite(ledLight_PIN, HIGH);
             delay(100);
             digitalWrite(ledLight_PIN, LOW);
                         
             digitalWrite(ledSmoke_PIN, HIGH);
             delay(100);
             digitalWrite(ledSmoke_PIN, LOW);
             
             digitalWrite(ledTemp_PIN, HIGH);
             delay(100);
             digitalWrite(ledTemp_PIN, LOW); 
     
             digitalWrite(ledHum_PIN, HIGH);
             delay(100);
             digitalWrite(ledHum_PIN, LOW);        
         
             Reset_AVR();
        }else
          Serial.println("Nessuna Campionatura, controllare sensore");
       n_camp = 0; //azzero le variabili per iniziare nuovamente il calcolo della media
       light_accum = 0;
       smoke_accum = 0.0;
       temp_accum = 0.0; 
       hum_accum = 0.0;  
       Serial.flush();    
       wifly.flushRx(1);       
    }


    if(time > ReadTime + Periodo_Lettura_Sensore){
        ReadTime = millis();
        light_accum += readLightSensor();
        smoke_accum += readSmokeSensor();
        temp_accum += readTempSensor();        
        hum_accum += readHumiditySensor();
        
        n_camp++;        
        Serial.print("Campione : ");
        Serial.print(n_camp);
        Serial.print(" - ");
        Serial.println(time);  
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

int readLightSensor(){
   int luce = analogRead(Light_PIN); // Inserisco il valore della lettura dell'input analogico sull'intero ValoreSensore
   luce = (5.0* luce * 200.0) / 1024.0;   
   return luce;
}

float readTempSensor(){
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float t = dht.readTemperature();

  // check if returns are valid, if they are NaN (not a number) then something went wrong!
  if (isnan(t)) {
    Serial.println("Failed to read from DHT");
  } else {
    return t;
   }  
}

float readHumiditySensor(){
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();

  // check if returns are valid, if they are NaN (not a number) then something went wrong!
  if (isnan(h)) {
    Serial.println("Failed to read from DHT");
  } else {
    return h;
   }  
}

void configWIFI(){
    println_P(PSTR("Starting"));
    print_P(PSTR("Free memory: "));
    Serial.println(wifly.getFreeMemory(),DEC);

    wifiSerial.begin(9600);
    if (!wifly.begin(&wifiSerial, &Serial)) {
        println_P(PSTR("Failed to start wifly"));
	//terminal();
        Reset_AVR();
    }

    char buf[32];
    /* Join wifi network if not already associated */
    if (!wifly.isAssociated()) {
	/* Setup the WiFly to connect to a wifi network */
	println_P(PSTR("Joining network"));
	wifly.setSSID(mySSID);
	wifly.setPassphrase(myPassword);
	wifly.enableDHCP();

	if (wifly.join()) {
	    println_P(PSTR("Joined wifi network"));
	} else {
	    println_P(PSTR("Failed to join wifi network"));
	    terminal();
	}
    } else {
        println_P(PSTR("Already joined network"));
    }

    print_P(PSTR("MAC: "));
    Serial.println(wifly.getMAC(buf, sizeof(buf)));
    print_P(PSTR("IP: "));
    Serial.println(wifly.getIP(buf, sizeof(buf)));
    print_P(PSTR("Netmask: "));
    Serial.println(wifly.getNetmask(buf, sizeof(buf)));
    print_P(PSTR("Gateway: "));
    Serial.println(wifly.getGateway(buf, sizeof(buf)));
    print_P(PSTR("SSID: "));
    Serial.println(wifly.getSSID(buf, sizeof(buf)));

    wifly.setDeviceID("Wifly-WebClient");
    print_P(PSTR("DeviceID: "));
    Serial.println(wifly.getDeviceID(buf, sizeof(buf)));

    if (wifly.isConnected()) {
        println_P(PSTR("Old connection active. Closing"));
	wifly.close();
    }
    
    if (wifly.open(serverName, 80)) {
        print_P(PSTR("Connected to "));
	Serial.println(serverName);

	Serial.println("WIFI ALREADY");
    } else {
        println_P(PSTR("Failed to connect"));
        Reset_AVR();
    }   
}

/* Print a string from program memory */
void print_P(const prog_char *str)
{
    char ch;
    while ((ch=pgm_read_byte(str++)) != 0) {
	Serial.write(ch);
    }
}

void println_P(const prog_char *str)
{
    print_P(str);
    Serial.println();
}

/* Connect the WiFly serial to the serial monitor. */
void terminal()
{
    while (1) {
	if (wifly.available() > 0) {
	    Serial.write(wifly.read());
	}


	if (Serial.available() > 0) {
	    wifly.write(Serial.read());
	}
    }
}

void InvioWIFIHttp(char server[], int porta, char pagina[], char username[], char password[], int idSensore, float dato)
{
  /* Send the request */
  //wifly.println("GET /arduino/pagina_server.php?username=&password=&sensore=5&dato=5 HTTP/1.0");
  //wifly.println();
     
  wifly.print("GET /arduino/");
  wifly.print(pagina);
  wifly.print("?username=");
  wifly.print(username);
  wifly.print("&password=");
  wifly.print(password);
  wifly.print("&sensore=");
  //wifly.print("5");
  wifly.print(idSensore);
  wifly.print("&dato=");
  //wifly.print("12");
  wifly.print(dato);
  wifly.println(" HTTP/1.1");
  wifly.print("Host: ");
  wifly.println(server);
  wifly.println();
  //wifly.println("Connection: Keep-Alive" );
  //wifly.println("Keep-Alive:timeout=100, max=5");
   
}

