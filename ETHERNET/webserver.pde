#include <SPI.h>
#include <Ethernet.h>
#include <string.h>


byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = { 192, 168, 1, 44 };
byte gateway[] = { 192, 168, 1, 1 };

char httpRead[128];			

Server server(80);			

void setup() {
  Ethernet.begin(mac, ip, gateway);
  server.begin();			
}

void loop() {
  
  Client client = server.available();
  if (client) { 			
    
    int indicebuffer = 0; 		
    
    if (client.connected() && client.available()) { 			
      httpRead[0] = client.read();					
      httpRead[1] = client.read();
      indicebuffer = 2;
      while (httpRead[indicebuffer-2] != '\r' && httpRead[indicebuffer-1] != '\n') {
	if (indicebuffer<128) httpRead[indicebuffer] = client.read();	
	indicebuffer++;
      }
      
      if ( strncmp(httpRead, "GET /home ", 10) == 0 ) {	
	client.print("HTTP/1.0 200 OK\nServer: arduino\nCache-Control: no-store, no-cache, must-revalidate\nPragma: no-cache\nConnection: close\nContent-Type: text/html\n\n<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" /></head><body></form><br/>");
	client.print("<b>Benvenuto. Cosa vuoi sapere?</b>");
	client.println("<br/><form action=\"submit\" method=\"get\"> Pin analogico: <input type=\"text\" name=\"apin\" value=\"5\" /> <br/> Tue note: <input type=\"text\" name=\"note\" value=\"La temperatura\" /> <br/>  <INPUT TYPE=submit VALUE=\"Invia\"></form><br/>");
	client.print("<br/><br/><a href=\"home\">Vai alla Homepage</a> <a href=\"info\">Informazioni sul programma</a></body></html>");   
      }
      
      else if ( strncmp(httpRead, "GET /info ", 10) == 0 ) {
	client.print("HTTP/1.0 200 OK\nServer: arduino\nCache-Control: no-store, no-cache, must-revalidate\nPragma: no-cache\nConnection: close\nContent-Type: text/html\n\n<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" /></head><body></form><br/>");
	client.print("<b>Ecco alcune informazioni:</b>");
	client.print("<br/>Questo programma implementa un semplice webserver che accetta argomenti tramite metodo GET.");
	client.print("<br/><br/><a href=\"home\">Vai alla Homepage</a> <a href=\"info\">Informazioni sul programma</a></body></html>"); 
      }
      
      else if (strncmp(httpRead, "GET /submit", 11) == 0) {			
		char apin[128];
		char note[128];
		char tmp[128];
		strcpy(tmp, "");
		strcpy(apin, "");
		strcpy(note, "");
		client.print("HTTP/1.0 200 OK\nServer: arduino\nCache-Control: no-store, no-cache, must-revalidate\nPragma: no-cache\nConnection: close\nContent-Type: text/html\n\n<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" /></head><body></form><br/>");
		int i = 17;
		int controllo = 0;
		while (i<strlen(httpRead) ){
			strcpy(tmp, "");
			strncat(tmp, &httpRead[i],1);
			if (strlen(note)==0){
				if (strcmp(tmp,"&")!=0 && strcmp(tmp," ")!=0) {
					strcat(apin, tmp);
				}
				if (strcmp(tmp,"&")==0 || strcmp(tmp," ")==0){
					i=i+6;
					strcpy(tmp, "");
					strncat(tmp, &httpRead[i],1);
					controllo = 1;
				}
			}
	  
		if (strlen(note)!=0 || controllo==1){
			controllo =0;
			if (strcmp(tmp,"&")!=0 && strcmp(tmp," ")!=0) strcat(note, tmp);
				if (strcmp(tmp,"&")==0 || strcmp(tmp," ")==0) i = strlen(httpRead);
			}
			i++;
		}
	
		client.print("Hai scelto di leggere il valore del pin ");
		client.print(apin);
	
		int miopin = atoi (apin);
		int temp = analogRead(miopin)*0.5;
		char temperatura[20];
		strcpy(temperatura, "");
		itoa(temp, temperatura, 10);
		client.print("<br/>"); 
		client.print("Il valore del pin risulta essere ");
		client.print(temperatura);
		client.print("<br/>"); 
		client.print("E come note hai scritto: ");
		client.print(note);
		client.print("<html><body><br/><br/><a href=\"home\">Vai alla Homepage</a> <a href=\"info\">Informazioni sul programma</a></body></html>");  
	}
      
      else client.print("HTTP/1.1 404 Not Found\nServer: arduino\nContent-Type: text/html\n\n<html><head></head><body><h1>Errore 404: Pagina non trovata!</h1><a href=\"home\">Vai alla Homepage</a></body></html>");
    }
    
    delay(5);  
    client.stop();	
  }
  
}

