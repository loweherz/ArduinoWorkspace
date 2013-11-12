#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>
 
#define maxLength 25
#define ledPin 6 //LED connesso al pin digitale 6

byte mac[] = { 0xC8, 0x0A, 0xA9, 0x71, 0xD5, 0x0C };
byte ip[] = { 169,254,0,201 };
File htmlFile;
EthernetServer server(80);
 
void setup()
{
  Ethernet.begin(mac, ip);
  server.begin();
  if (!SD.begin(4)) { return; }
  pinMode(ledPin, OUTPUT);  //Imposto il LED come output
}
 
void loop()
{
  char* file_to_load = "index.html";
  String inString = String(maxLength);
 
  EthernetClient client = server.available();
  if (client) {
    digitalWrite(ledPin, HIGH);  //Accendo il LED
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (inString.length() < maxLength) {
          inString += c;
        }
        if (c == '\n' && currentLineIsBlank) {
          if (inString.indexOf(".html") > -1) {
            String new_file_load;
            int rootIndex = inString.indexOf("/");
            new_file_load = inString.substring((rootIndex+1), (rootIndex+13));
 
            int endIndex = new_file_load.indexOf(" ");
            if (endIndex > -1) {
              new_file_load = new_file_load.substring(0, endIndex);
            }
            if (new_file_load != "")  {
              new_file_load.toCharArray(file_to_load,12);
            }
          }
 
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println(); 
           
          //read_file( "header.htm",client );
          //read_file( file_to_load,client );
          //read_file( "footer.htm",client );
 
          break;
        }
        if (c == '\n') {
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          currentLineIsBlank = false;
        }
      }
    }
    delay(1);
    client.stop();
  }
}
 
void read_file( char* page_html, EthernetClient client )
{
  htmlFile = SD.open( page_html );
  if (htmlFile) {
    while (htmlFile.available()) {
        client.write(htmlFile.read());
    }
    // close the file:
    htmlFile.close();
  }
}
