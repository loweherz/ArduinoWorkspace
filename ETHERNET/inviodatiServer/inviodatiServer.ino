#include <SPI.h>
#include <Ethernet.h>

#define Sensor_PIN A0 //imposto il pin a cui è connesso il sensore
#define led_PIN 8
#define Periodo_Invio_Dati 10000 //tempo minimo tra un'invio sul web e l'altro.(ms)
#define Periodo_Lettura_Sensore 2000 //tempo minimo tra una lettura del sensore e l'altra (ms)

#define id_sensore 1

char username[] = "root";//username per login sito
char password[] = "";//password per login sito

byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0x1E, 0x93 };
byte ip[] = { 192,168,0,10 };

//IPAddress serverName(127,0,0,1);//indirizzo IP del server a cui connettersi
char serverName[] = "192.168.0.200";//URL del server a cui connettersi
#define serverPort 80 //porta di connessione
char pageName[] = "pagina_server.php";//nome pagina php per la ricezione dei dati

EthernetClient client;//dichiaro una variabile globale per il client ethernet

double accum = 0.0;

long n_camp = 0;
float avg_sensor = 0.0;

unsigned long time = 0;
unsigned long SendTime = 0;
unsigned long ReadTime = 0;


void setup()
{
  // start the serial library:
  Serial.begin(9600);
  pinMode(led_PIN, OUTPUT);
  // start the Ethernet connection:
  Serial.println("Configuro la connessione con DHCP...");
  Ethernet.begin(mac, ip);  //configuro lo shield ethernet
  Serial.println("Connessione Configurata.");
  delay(1000);//aspetto un secondo per far avviare lo shield ethernet
  Serial.println("Programma Avviato, Setup Terminato");

}


void loop()
{
    time = millis();  

    if(time > SendTime + Periodo_Invio_Dati){
       SendTime = millis();
       avg_sensor = float(accum / double(n_camp));//calcolo la media delle lettura     
       if(n_camp > 0)
       {
         Serial.println("connessione...");
         Serial.println(serverName);
         if (client.connect(serverName, serverPort)) //connessione al server per invio lettura sensore temperatura
         {
              digitalWrite(led_PIN, HIGH);
              Serial.println("connesso");
              Serial.println("inizio primo invio");
              InvioHttp(serverName,serverPort,pageName,username,password,id_sensore,avg_sensor);
              client.stop();
              Serial.println("Client Disconnesso");
              Serial.println("fine primo invio");
              digitalWrite(led_PIN, LOW);
          } 
          else
            Serial.println("Errore Prima Connessione");
        }
        else
          Serial.println("Nessuna Campionatura, controllare sensore");
       n_camp = 0; //azzero le variabili per iniziare nuovamente il calcolo della media
       accum = 0.0;
    }


    if(time > ReadTime + Periodo_Lettura_Sensore){
        ReadTime = millis();
        int sensorValue = analogRead(Sensor_PIN);
        accum += sensorValue;
        n_camp++;
        Serial.print("Value: ");
        Serial.print(sensorValue);
        Serial.print(" - ");
        Serial.print("Campione : ");
        Serial.print(n_camp);
        Serial.print(" - ");
        Serial.println(time);  
    }
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
