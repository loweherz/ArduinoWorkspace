#include <WiFlyHQ.h>
#include <SoftwareSerial.h>
#include "DHT.h"
#include <SD.h>

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

#define Periodo_Invio_Dati 30000 //10s = tempo minimo tra un'invio sul web e l'altro.(ms)

#define id_Light 2000
#define id_Smoke 3000
#define id_Temp 4000
#define id_Hum 5000

/*WIFI SETTING*/
WiFly wifly;

/* Change these to match your WiFi network */
const char mySSID[] = "tsunami";
const char myPassword[] = "12345678912345678912345678";

char serverName[] = "monfortino.ce.unipr.it";//URL del server a cui connettersi
#define serverPort 80 //porta di connessione
//char pageName[] = "pagina_server.php";//nome pagina php per la ricezione dei dati
char* macStr;

SoftwareSerial wifi(8,9);

int light_accum = 0; 
float temp_accum = 0.0;
float smoke_accum = 0.0;
float hum_accum = 0.0;

unsigned long time = 0;
unsigned long SendTime = 0;

char lightBuffer[8];    
char smokeBuffer[8];    
char tempBuffer[8];    
char humBuffer[8];
char timestamp[8];

char jsonMsgHead[128];
char jsonMsgBody[128];

int online;

void setup()
{
  // start the serial library:
  Serial.begin(9600);
  
  if(configWIFI()>0) online = 1;
  else online = 0;
  
  pinMode(ledLight_PIN, OUTPUT);
  pinMode(ledSmoke_PIN, OUTPUT);
  pinMode(ledTemp_PIN, OUTPUT);
  pinMode(ledHum_PIN,OUTPUT);
  Serial.println("Connessione Configurata.");
  delay(1000);//aspetto un secondo per far avviare lo shield ethernet
  Serial.println("Programma Avviato, Setup Terminato!");
  randomSeed(analogRead(A2));
  
  const int chipSelect = 4;
  
  Serial.print("Initializing SD card...");
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(10, OUTPUT);
  
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");

}


void loop()
{
    time = millis();
        
    if(time > SendTime + Periodo_Invio_Dati){
             SendTime = millis();
             light_accum = readLightSensor();
             smoke_accum = readSmokeSensor();
             readDHTSensor();              
             
             //CONVERTO I VALORI IN STRINGA
             dtostrf(light_accum, 5, 2, lightBuffer);
             dtostrf(smoke_accum, 5, 2, smokeBuffer);
             dtostrf(temp_accum, 5, 2, tempBuffer);
             dtostrf(hum_accum, 5, 2, humBuffer);
             sprintf(timestamp, "%d", random(1, 32000)); 
             Serial.println(timestamp);
                          
             Serial.println("invio lettura sensori");
             Serial.println("CREO JSON");
             //sprintf(jsonMsg, "{\"timestamp\":%s,\"checksum\":\"%s\",\"mac\":\"%s\",\"light\":%s,\"gas\":%s,\"temperature\":%s,\"humidity\":%s}\0", timestamp, "5EB63BBBE01EEED093CB22BB8F5ACDC3", "00:06:66:71:d2:68", lightBuffer, smokeBuffer, tempBuffer, humBuffer);
             sprintf(jsonMsgHead,"{\"timestamp\":%s,\"checksum\":\"%s\",\"mac\":\"%s\"\0", timestamp, "5EB63BBBE01EEED093CB22BB8F5ACDC3", "00:06:66:71:d2:68");
             sprintf(jsonMsgBody,",\"light\":%s,\"gas\":%s,\"temperature\":%s,\"humidity\":%s}\0", lightBuffer, smokeBuffer, tempBuffer, humBuffer);
             int i; for(i=0; jsonMsgHead[i]!=0; i++);
             i++;
             for(int j=0;jsonMsgBody[j]!=0; j++, i++);
             Serial.println(++i);
             Serial.print(jsonMsgHead);
             Serial.println(jsonMsgBody);
             if(online==1){
                 Serial.println("ONLINE -> Server");
                 InvioWIFIHttp(jsonMsgHead,jsonMsgBody, i);           
             }else{
                 Serial.println("OFFLINE -> SD");
                 InvioSD(jsonMsgHead,jsonMsgBody, i);
             }
             Serial.println("fine invio"); 
        
             delay(2000);
       
             Reset_AVR(); 
       
             Serial.flush();    
             wifly.flushRx(1);             
    }   
}

//LETTURA SENSORI
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

int configWIFI(){
    Serial.println("Starting");
    Serial.print("Free memory: ");
    Serial.println(wifly.getFreeMemory(),DEC);

    wifi.begin(9600);
    if (!wifly.begin(&wifi, &Serial)) {
        Serial.println("Failed to start wifly");
        return -1;
	//Reset_AVR();
    }

    char buf[32];
    /* Join wifi network if not already associated */
    if (!wifly.isAssociated()) {
	/* Setup the WiFly to connect to a wifi network */
	Serial.println("Joining network");
	wifly.setSSID(mySSID);
	//wifly.setPassphrase(myPassword);
        wifly.setKey(myPassword);
	wifly.enableDHCP();

	if (wifly.join()) {
	    Serial.println("Joined wifi network");
	} else {
	    Serial.println("Failed to join wifi network");
	    return -1;
            //Reset_AVR();
	}
    } else {
        Serial.println("Already joined network");
    }

    Serial.print("MAC: ");
    macStr = (char *)(wifly.getMAC(buf, sizeof(buf)));
    Serial.println(macStr);
    Serial.print("IP: ");
    Serial.println(wifly.getIP(buf, sizeof(buf)));
    Serial.print("Netmask: ");
    Serial.println(wifly.getNetmask(buf, sizeof(buf)));
    Serial.print("Gateway: ");
    Serial.println(wifly.getGateway(buf, sizeof(buf)));

    wifly.setDeviceID("Wifly-DSG");
    Serial.print("DeviceID: ");
    Serial.println(wifly.getDeviceID(buf, sizeof(buf)));

    if (wifly.isConnected()) {
        Serial.println("Old connection active. Closing");
	wifly.close();
    }
    
    if (wifly.open(serverName, serverPort)) {
        Serial.print("Connected to ");
	Serial.println(serverName);

	Serial.println("WIFI ALREADY");
    } else {
        Serial.println("Failed to connect");
        return -1;
        //Reset_AVR();
    }
    return 1;   
}

void InvioWIFIHttp(char* jsonStringHead,char* jsonStringBody, int lungh)
{
  Serial.println("...sending...");
  /*
  sprintf(httpreq,"POST /dsgm2m/api/m2m.php HTTP/1.0\r\n"
                      "User-Agent: Arduino Board\r\n"
                      "Content-type: application/x-www-form-urlencoded\r\n"
                      "Content-Length: %d\r\n\r\n"
                      "%s",lungh,jsonString);
  
  Serial.print(".....");
  Serial.println(httpreq);  */
  
  wifly.print("POST /dsgm2m/api/m2m.php HTTP/1.0\r\n");
  wifly.print("Content-Length: ");
  wifly.print(lungh);
  wifly.print("\r\n\r\n");
  wifly.print(jsonStringHead);
  wifly.print(jsonStringBody);
  
  Serial.println();
  //wifly.print(httpreq);
  /*
  Serial.println(wifly.available());
  
     if (wifly.available() > 0) {
       int readVal = 0;
    
       while((readVal = wifly.read()) != -1)
         Serial.print((char)readVal);
    }*/   
}

void InvioSD(char* jsonStringHead,char* jsonStringBody, int lungh)
{
  Serial.println("...sending...");
  
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.print(lungh);
    dataFile.print("=");
    dataFile.print(jsonStringHead);
    dataFile.println(jsonStringBody);
    dataFile.close();
    // print to the serial port too:
    Serial.println("Writing OK");
  }  
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
  
    
  
}
