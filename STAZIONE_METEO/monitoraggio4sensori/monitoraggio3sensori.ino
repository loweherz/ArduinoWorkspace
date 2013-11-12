#include <SPI.h>
#include <Ethernet.h>

#define Light_PIN A1 //imposto il pin a cui è connesso il sensore
#define Smoke_PIN A2 //imposto il pin a cui è connesso il sensore
#define DHT22_PIN 2 //imposto il pin a cui è connesso il sensore

#define ledLight_PIN 7
#define ledSmoke_PIN 8
#define ledTemp_PIN 9

#define Periodo_Invio_Dati 10000 //30s = tempo minimo tra un'invio sul web e l'altro.(ms)
#define Periodo_Lettura_Sensore 2000 //2s = tempo minimo tra una lettura del sensore e l'altra (ms)

#define id_Light 2000
#define id_Smoke 3000
#define id_Temp 4000

char username[] = "root";  //username per login sito
char password[] = "";      //password per login sito

byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0x1E, 0x93 };
byte ip[] = { 192,168,0,10 };

//IPAddress serverName(127,0,0,1);//indirizzo IP del server a cui connettersi
char serverName[] = "192.168.0.200";//URL del server a cui connettersi
#define serverPort 80 //porta di connessione
char pageName[] = "pagina_server.php";//nome pagina php per la ricezione dei dati

EthernetClient client;//dichiaro una variabile globale per il client ethernet

int light_accum = 0; 
float temp_accum = 0.0;
float smoke_accum = 0.0;

long n_camp = 0;

float avg_light = 0.0;
float avg_smoke = 0.0;
float avg_temp = 0.0;

unsigned long time = 0;
unsigned long SendTime = 0;
unsigned long ReadTime = 0;


void setup()
{
  // start the serial library:
  Serial.begin(9600);
  pinMode(ledLight_PIN, OUTPUT);
  pinMode(ledSmoke_PIN, OUTPUT);
  pinMode(ledTemp_PIN, OUTPUT);
  // start the Ethernet connection:
  Serial.println("...Configuro la connessione ethernet...");
  Ethernet.begin(mac, ip);  //configuro lo shield ethernet
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
       if(n_camp > 0)
       {
         Serial.println("connessione...");
         Serial.println(serverName);         
          if (client.connect(serverName, serverPort)) //connessione al server per invio lettura sensore temperatura
          {
              digitalWrite(ledLight_PIN, HIGH);
              Serial.println("connesso");
              Serial.println("invio light sensor");
              InvioHttp(serverName,serverPort,pageName,username,password,id_Light,avg_light);
              client.stop();
              Serial.println("Client Disconnesso");
              Serial.println("fine invio");
              digitalWrite(ledLight_PIN, LOW);
          } 
          else
            Serial.println("Errore Seconda Connessione");
          if (client.connect(serverName, serverPort)) //connessione al server per invio lettura sensore temperatura
          {
              digitalWrite(ledSmoke_PIN, HIGH);
              Serial.println("connesso");
              Serial.println("invio smoke sensor");
              InvioHttp(serverName,serverPort,pageName,username,password,id_Smoke,avg_smoke);
              client.stop();
              Serial.println("Client Disconnesso");
              Serial.println("fine invio");
              digitalWrite(ledSmoke_PIN, LOW);
          } 
          else
            Serial.println("Errore Terza Connessione");
          if (client.connect(serverName, serverPort)) //connessione al server per invio lettura sensore temperatura
          {
              digitalWrite(ledTemp_PIN, HIGH);
              Serial.println("connesso");
              Serial.println("invio temp sensor");
              InvioHttp(serverName,serverPort,pageName,username,password,id_Temp,avg_temp);
              client.stop();
              Serial.println("Client Disconnesso");
              Serial.println("fine invio");
              digitalWrite(ledTemp_PIN, LOW);
          } 
          else
            Serial.println("Errore Quarta Connessione");
        }
        else
          Serial.println("Nessuna Campionatura, controllare sensore");
       n_camp = 0; //azzero le variabili per iniziare nuovamente il calcolo della media
       light_accum = 0;
       smoke_accum = 0.0;
       temp_accum = 0.0;
    }


    if(time > ReadTime + Periodo_Lettura_Sensore){
        ReadTime = millis();
        light_accum += readLightSensor();
        smoke_accum += readSmokeSensor();
        temp_accum += readTempSensor();        
        
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
  //float h = dht.readHumidity();
  //float t = dht.readTemperature();
  float h = 0.0;
  float t = 0.0;
  
  // check if returns are valid, if they are NaN (not a number) then something went wrong!
  //if (isnan(t) || isnan(h)) {
    //Serial.println("Failed to read from DHT");
  //  temp=0.0;
  //  humid=0.0;
  //} else {    
  //  temp = t;
  //  humid = h;
  //}  
  return t;
}

void InvioHttp(char server[], int porta, char pagina[], char username[], char password[], int idSensore, float dato)
{
  //client.print("GET http://");// GET http://'server':'porta'/'pagina'?username=nome_utente&password=password&sensore=numero_sensore&dato=lettura_sensore  HTTP/1.1  client.print(server);
  //client.print(server);
  //client.print(":");
  //client.print(porta);
  client.print("GET /arduino/");
  client.print(pagina);
  client.print("?username=");
  client.print(username);
  client.print("&password=");
  client.print(password);
  client.print("&sensore=");
  client.print(idSensore);
  client.print("&dato=");
  client.print(dato);
  client.println(" HTTP/1.1");
  client.print("Host: ");
  client.println(server);
  client.println();
}
