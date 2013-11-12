#include <SPI.h>
#include <Ethernet.h>


byte mac[] = { 
  0x90, 0xA2, 0xDA, 0x0D, 0x1D, 0x93 };
byte ip[] = { 
  192 ,168 ,0 ,14 };
byte gateway[] = { 
  192 ,168 ,0 ,1 };
byte subnet[] = { 
  255 ,255 ,255 ,0 };
EthernetServer server(8080); 

String readString = String(30);


void setup(){
  Ethernet.begin(mac, ip, gateway, subnet);
  delay(1000);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  Serial.begin(9600);
} 

void loop(){
  EthernetClient client = server.available();
  if (client) {
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (readString.length() < 30)
        {
          readString = readString + c;
        }
        if (c == '\n') {
          Serial.print(readString);
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();


          if(readString.startsWith("GET /?out=13&status=1"))
          {
            Serial.print("\n 13 HIGH \n");
            digitalWrite(13, HIGH);
            client.print("{\"status\" : \"1\" , \"out\" : \"");
            client.print(13);
            client.print("\"}");
          }
          if(readString.startsWith("GET /?out=13&status=0"))
          {
            Serial.print("\n 13 LOW \n");
            digitalWrite(13, LOW);
            client.print("{\"status\" : \"0\" , \"out\" : \"");
            client.print(13);
            client.print("\"}");
          }


          if(readString.startsWith("GET /?out=all"))
          {
            Serial.print("\n OUT ALL\n");
            client.print("{\"ip\" : \"192.168.0.14\", ");
            client.print("\"devices\" : ");
            client.print("[{ \"type\" : \"light\", \"name\" : \"led\", \"out\" : \"");
            client.print("13");
            client.print("\"}");
            client.print("]}");
          }
          readString="";
          client.stop();
        }
      }
    }
  }
}

