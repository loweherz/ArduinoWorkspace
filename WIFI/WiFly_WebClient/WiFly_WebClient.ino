
// (Based on Ethernet's WebClient Example)

#include <SPI.h>
#include <WiFly.h>
#include <SoftwareSerial.h>

#include "Credentials.h"


byte server[] = { 192, 168, 1, 2 }; 

//Client client(server, 80);

WiFlyClient client;
SoftwareSerial Serialnew(2,3);

void setup() {
  
  Serial.begin(9600);
  Serialnew.begin(9600);
  Serialnew.println("CONNESSIONE IN CORSO");
  WiFly.begin();
  
  if (!WiFly.join(ssid, passphrase)) {
    Serialnew.println("Association failed.");
    while (1) {
      // Hang on failure.
    }
  }  

  Serialnew.println("connecting...");

  if (client.connect("localhost", 80)) {
    Serialnew.println("connected");
    client.println("GET /arduino/pagina_server.php?username=&password=&sensore=5&dato=5 HTTP/1.0");
    client.println();
  } else {
    Serialnew.println("connection failed");
  }
  
}

void loop() {
  if (client.available()) {
    char c = client.read();
    Serialnew.print(c);
  }
  
  if (!client.connected()) {
    Serialnew.println();
    Serialnew.println("disconnecting.");
    client.stop();
    for(;;)
      ;
  }
}


