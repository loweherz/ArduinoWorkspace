#include <String.h>
#include <SPI.h>
#include <Ethernet.h>
 
/* ***
COMPILARE CON ARDUINO 1.0 o successive
controllare un attuatore via web
ARDUINO UNO
ETHERNET SHIELD
by GianniFavilli.it - www.giannifavilli.it
*** */
 
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; // mac address
byte ip[] = { 169,254,0,210 }; // ip arduino internet in
EthernetServer server(80); //server port
 
int outPin = 6; // pin attuatore
String readString; //string
boolean LEDON = false; // flag status attuatore
 
void setup(){
  Ethernet.begin(mac, ip);
  pinMode(outPin, OUTPUT);
  Serial.begin(9600);
}
 
void loop(){ 
  /*inizio client*/
  EthernetClient client = server.available();
  if (client) {
  boolean currentLineIsBlank = true;
  while (client.connected()) {
    if (client.available()) {
      char c = client.read();
      readString.concat(c); //store characters to string
      //if HTTP request has ended            
      if (c == '\n' && currentLineIsBlank) {
        Serial.print(readString);
        if(readString.indexOf("L=1") > 0) {// lettura del valore se il LED si deve accendere
          // il led sarà acceso
          digitalWrite(outPin, HIGH); // accendo il led
          LEDON = true;
          Serial.print("ON pin ");
          Serial.println(outPin);
          }
        else if(readString.indexOf("L=0") > 0)
          {
          //il led sarà spento
          digitalWrite(outPin, LOW); //sengo il led
          LEDON = false;
          Serial.print("OFF pin ");
          Serial.println(outPin);
        }
        else {
          if(LEDON) digitalWrite(outPin, HIGH); 
          else digitalWrite(outPin, LOW);
        }
 
        // COSTRUZIONE PAGINA HTML
        client.println("HTTP/1.1 200 OK.....");
        client.println("Content-Type: text/html");
        client.println();
        // inizializzo pagina (da togliere se uso ajax)
        client.print("<html><head><title>ARDUINO Controllo Led via WEB</title><meta http-equiv='Content-Type' content='text/html; charset=iso-8859-1' ></head><body>");
        //inizai il body
        client.println("<div style='width:360px; height:640px;'>"); //risoluzione per nokia 5800 640x360
         
        client.println("<h1>CONTROLLO ATTUATORI via internet</h1>");
        client.println("<hr />");
        client.print("<h1>PIN control n. ");
        client.print(outPin);
        client.println("</h1>");
        client.println("<br />");
        //scrivo il LED status
        client.print("<font size='5'>PIN status: ");
          if (LEDON) {
              client.println("<span style='color:green; font-weight:bold;'>ON</span></font>");
            }
            else
            {
              client.println("<span style='color:grey; font-weight:bold;'>OFF</span></font>");
          }
        client.print("<h2><a href='/?L=1'>ACCENDI</a> | <a href='/?L=0'>SPEGNI</a></h2>");
 
        // chiudo il div
        client.println("</div>");
        // chiudo pagina da togliere se uso ajax
        client.println("</body></html>");
 
        // pulisco la stringa per la successiva lettura
        readString="";
        //fermo il client
        delay(1000); //NECESSARIO PER CARICARE LA PAGINA AL CLIENT
        client.stop();
 
        } //if c == /n
    } // if client available
  } // while client connesso
} // FINE if client        
 
} // fine loop
