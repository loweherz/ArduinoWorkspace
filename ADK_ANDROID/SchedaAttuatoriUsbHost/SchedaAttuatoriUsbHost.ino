#include <UsbHost.h>
#include <AndroidAccessory.h>
#include <SPI.h>
#include <Ethernet.h>
#include <Servo.h> 

#include <avr/io.h>
#include <avr/wdt.h>
#define Reset_AVR() wdt_enable(WDTO_30MS); while(1) {}
#define Reset_After_1hour 3600000

char accessoryName[] = "Mega_ADK"; // your Arduino board
char companyName[] = "Arduino";
bool conn = false;

MAX3421E Max;
UsbHost Usb;
AndroidAccessory usb_android(companyName, accessoryName);

byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0x1E, 0x52 };
//byte ip[] = { 192,168,1,10 };

char buffer[32];
char bufferRisp[256];

char serverName[] = "monfortino.ce.unipr.it";//URL del server a cui connettersi
#define serverPort 80 //porta di connessione

EthernetClient client;//dichiaro una variabile globale per il client ethernet

unsigned long time = 0;

char valBuffer[8];
char timestamp[8];

char jsonMsg[256];

int sizeRisp = 0;
int richiedorisposta = 0;
int i;

Servo myservo;  // create servo object to control a servo 
 
int potpin = A1;  // analog pin used to connect the potentiometer
int valServo;    // variable to read the value from the analog pin 

const int buttonPin = 2;     // the number of the pushbutton pin
const int fanPin =  9;      // the number of the LED pin
 
int valFan=0; //val will be used to store state of pin 
int old_val=0;

int buttonState= 0;


void setup()
{
  usb_android.begin(); 
  Serial.begin( 9600 );
  Serial.println("Start");
  Max.powerOn();
  Serial.println("USB HOST SHIELD"); 
  Serial.println("...Configuro la connessione ethernet...");
  Ethernet.begin(mac);  //configuro lo shield ethernet
  Serial.println("Connessione Configurata.");
  delay(1000);//aspetto un secondo per far avviare lo shield ethernet
  Serial.println("Programma Avviato, Setup Terminato!");
  Serial.print("My IP address: ");
  Serial.println(Ethernet.localIP());

  // initialize the LED pin as an output:
  pinMode(fanPin, OUTPUT);      
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT); 
  
  myservo.attach(6);  // attaches the servo on pin 9 to the servo object 
  
}

void loop(){
    time = millis();
    
    setServo();
    setFan();
  
    if(usb_android.isConnected() && !conn){
        conn = true;
        if (!usbcheck()) Serial.println("USB connection test failed."); 
    }    
    
    if(!usb_android.isConnected()){ 
        conn = false;     
    }
    
    if(time>Reset_After_1hour){
        Serial.println("Goodbye!!");
        Reset_AVR();
    }
}

void setServo(){
  valServo = analogRead(potpin);            // reads the value of the potentiometer (value between 0 and 1023) 
  valServo = map(valServo, 0, 1023, 0, 179);     // scale it to use it with the servo (value between 0 and 180) 
  myservo.write(valServo);                  // sets the servo position according to the scaled value 
  delay(15);
}

void setFan(){
  valFan = digitalRead(buttonPin);
  //check if input is HIGH
 
  if ((valFan == HIGH) && (old_val == LOW)) {
      buttonState = 1 - buttonState;
      delay(50);
  }
 
  old_val = valFan;
 
  if (buttonState == 1) {     
    // turn LED on:    
    digitalWrite(fanPin, HIGH);  
  } 
  else {
    // turn LED off:
    digitalWrite(fanPin, LOW); 
  }
}
/* Test USB connectivity */
bool usbcheck(){
  bool exit=false;
  byte rcode;
  byte usbstate;
    Max.powerOn();
    delay( 200 );
    Serial.println("\r\nUSB Connectivity test. Waiting for device connection... ");
    while(!exit){      
      delay( 200 );
      Max.Task();
      Usb.Task();
      usbstate = Usb.getUsbTaskState();
      //--DEBUG Serial.println(usbstate);
      switch( usbstate ) {
        case( USB_ATTACHED_SUBSTATE_RESET_DEVICE ):
          Serial.println("\r\nDevice connected. Resetting");          
          break;
        case( USB_ATTACHED_SUBSTATE_WAIT_SOF ): 
          Serial.println("\r\nReset complete. Waiting for the first SOF...");
          //delay( 1000 );
          break;  
        case( USB_ATTACHED_SUBSTATE_GET_DEVICE_DESCRIPTOR_SIZE ):
          Serial.println("\r\nSOF generation started. Enumerating device.");
          break;
        case( USB_STATE_ADDRESSING ):
          Serial.println("\r\nSetting device address");
          //delay( 100 );
          break;
        case( USB_STATE_RUNNING ):
          //delay( 1000 );
          Serial.println("\r\nGetting device descriptor");
          rcode = getdevdescr( 1 );
          exit = true;
            if( rcode ) {
              Serial.print("\r\nError reading device descriptor. Error code ");
              print_hex( rcode, 8 );
            }
            else {
              Serial.println("\r\n\nAll tests passed.");
              Usb.setUsbTaskState(0xF0); 
              //while(1);
            }          
          break;
        case( USB_STATE_ERROR ):
          Serial.println("\r\nUSB state machine reached error state");
          break;
        default:{          
          break;
        }
    }//switch
    }//while(1)
}
/* Get device descriptor */
byte getdevdescr( byte addr )
{
  USB_DEVICE_DESCRIPTOR buf;
  byte rcode;
  rcode = Usb.getDevDescr( addr, 0, 0x12, ( char *)&buf );
  if( rcode ) {
    return( rcode );
  }
  Serial.print("\r\nDevice descriptor: ");
  Serial.print("\r\nDescriptor Length:\t");
  print_hex( buf.bLength, 8 );
  Serial.print("\r\nUSB version:\t");
  print_hex( buf.bcdUSB, 16 );
  Serial.print("\r\nVendor ID:\t");
  Serial.print(buf.idVendor);
  //print_hex( buf.idVendor, 16 );
  Serial.print("\r\nProduct ID:\t");
  Serial.print(buf.idProduct);
  //print_hex( buf.idProduct, 16 );
  Serial.print("\r\nRevision ID:\t");
  Serial.print(buf.bcdDevice);
  //print_hex( buf.bcdDevice, 16 );
  sendDataToEthernet(buf.idVendor, buf.idProduct, buf.bcdDevice);
  return( 0 );
}

void sendDataToEthernet(int idVendor, int idProduct, int bcdDevice){
   sprintf(timestamp, "%d", random(1, 32000));
      
   sprintf(jsonMsg,"{\"timestamp\":\"%s\",\"checksum\":\"%s\",\"mac\":\"%s\",\"usbhost\":%d.%d.%d}\0", timestamp, "5EB63BBBE01EEED093CB22BB8F5ACDC3", "90:A2:DA:0D:1E:52", idVendor, idProduct, bcdDevice);
   for(i=0; jsonMsg[i]!=0; i++);
   i++;
   
   if (client.connect(serverName, serverPort)) //connessione al server per invio lettura sensore temperatura
          {
              Serial.println("CONNESSO...");
              delay(200);
              InvioJSONRequest(jsonMsg, i);
              delay(200);
              client.stop();
              Serial.println("...FINE INVIO!!!");              
          }
   
}

void InvioJSONRequest(char* jsonString, int lungh)
{
  client.print("POST /dsgm2m/api/m2m.php HTTP/1.0\r\n");
  client.print("Content-Length: ");
  client.print(lungh);
  client.print("\r\n\r\n");
  client.print(jsonString); 
}

void print_hex(int v, int num_places)
{
  int mask=0, n, num_nibbles, digit;

  for (n=1; n<=num_places; n++)
  {
    mask = (mask << 1) | 0x0001;
  }
  v = v & mask; // truncate v to specified number of places

  num_nibbles = num_places / 4;
  if ((num_places % 4) != 0)
  {
    ++num_nibbles;
  }

  do
  {
    digit = ((v >> (num_nibbles-1) * 4)) & 0x0f;
    Serial.print(digit, HEX);
  } 
  while(--num_nibbles);
}
