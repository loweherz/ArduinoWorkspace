#include <UsbHost.h>
#include <AndroidAccessory.h>

char accessoryName[] = "Mega_ADK"; // your Arduino board
char companyName[] = "Arduino";
bool conn = false;

MAX3421E Max;
UsbHost Usb;
AndroidAccessory usb_android(companyName, accessoryName);

#define  LED3_RED       2
#define  LED3_GREEN     4
#define  LED3_BLUE      3

#define  LED2_RED       5
#define  LED2_GREEN     7
#define  LED2_BLUE      6

#define  LED1_RED       8
#define  LED1_GREEN     10
#define  LED1_BLUE      9

#define  SERVO1         11
#define  SERVO2         12
#define  SERVO3         13

#define  TOUCH_RECV     14
#define  TOUCH_SEND     15

#define  RELAY1         A0
#define  RELAY2         A1

#define  LIGHT_SENSOR   A2
#define  TEMP_SENSOR    A3

#define  BUTTON1        A6
#define  BUTTON2        A7
#define  BUTTON3        A8

#define  JOY_SWITCH     A9      // pulls line down when pressed
#define  JOY_nINT       A10     // active low interrupt input
#define  JOY_nRESET     A11     // active low reset output

void init_buttons()
{
	pinMode(BUTTON1, INPUT);
	pinMode(BUTTON2, INPUT);
	pinMode(BUTTON3, INPUT);
	pinMode(JOY_SWITCH, INPUT);

	// enable the internal pullups
	digitalWrite(BUTTON1, HIGH);
	digitalWrite(BUTTON2, HIGH);
	digitalWrite(BUTTON3, HIGH);
	digitalWrite(JOY_SWITCH, HIGH);
}


void init_relays()
{
	pinMode(RELAY1, OUTPUT);
	pinMode(RELAY2, OUTPUT);
}


void init_leds()
{
	digitalWrite(LED1_RED, 1);
	digitalWrite(LED1_GREEN, 1);
	digitalWrite(LED1_BLUE, 1);

	pinMode(LED1_RED, OUTPUT);
	pinMode(LED1_GREEN, OUTPUT);
	pinMode(LED1_BLUE, OUTPUT);

	digitalWrite(LED2_RED, 1);
	digitalWrite(LED2_GREEN, 1);
	digitalWrite(LED2_BLUE, 1);

	pinMode(LED2_RED, OUTPUT);
	pinMode(LED2_GREEN, OUTPUT);
	pinMode(LED2_BLUE, OUTPUT);

	digitalWrite(LED3_RED, 1);
	digitalWrite(LED3_GREEN, 1);
	digitalWrite(LED3_BLUE, 1);

	pinMode(LED3_RED, OUTPUT);
	pinMode(LED3_GREEN, OUTPUT);
	pinMode(LED3_BLUE, OUTPUT);
}

void init_joystick(int threshold);

byte b1, b2, b3, b4, c;

void setup()
{
  usb_android.begin(); 
  Serial.begin( 9600 );
  Serial.println("Start");
  Max.powerOn();
  Serial.println("USB HOST SHIELD");  
  //if (!usbtest()) Serial.println("USB connection test failed."); 
  
  init_leds();
	init_relays();
	init_buttons();
	
}

void loop(){
  byte err;
	byte idle;
	static byte count = 0;
	byte msg[3];
	long touchcount;  
  if(usb_android.isConnected() && !conn){
    conn = true;
    if (!usbcheck()) Serial.println("USB connection test failed.");     
    int len = usb_android.read(msg, sizeof(msg), 1);
		int i;
		byte b;
		uint16_t val;
		int x, y;
		char c0;

		if (len > 0) {
			// assumes only one command per packet
			if (msg[0] == 0x2) {
				if (msg[1] == 0x0)
					analogWrite(LED1_RED, 255 - msg[2]);
				else if (msg[1] == 0x1)
					analogWrite(LED1_GREEN, 255 - msg[2]);
				else if (msg[1] == 0x2)
					analogWrite(LED1_BLUE, 255 - msg[2]);
				else if (msg[1] == 0x3)
					analogWrite(LED2_RED, 255 - msg[2]);
				else if (msg[1] == 0x4)
					analogWrite(LED2_GREEN, 255 - msg[2]);
				else if (msg[1] == 0x5)
					analogWrite(LED2_BLUE, 255 - msg[2]);
				else if (msg[1] == 0x6)
					analogWrite(LED3_RED, 255 - msg[2]);
				else if (msg[1] == 0x7)
					analogWrite(LED3_GREEN, 255 - msg[2]);
				else if (msg[1] == 0x8)
					analogWrite(LED3_BLUE, 255 - msg[2]);
			} else if (msg[0] == 0x3) {
				if (msg[1] == 0x0)
					digitalWrite(RELAY1, msg[2] ? HIGH : LOW);
				else if (msg[1] == 0x1)
					digitalWrite(RELAY2, msg[2] ? HIGH : LOW);
			}
		}

		msg[0] = 0x1;

		b = digitalRead(BUTTON1);
		if (b != b1) {
			msg[1] = 0;
			msg[2] = b ? 0 : 1;
			usb_android.write(msg, 3);
			b1 = b;
		}

		b = digitalRead(BUTTON2);
		if (b != b2) {
			msg[1] = 1;
			msg[2] = b ? 0 : 1;
			usb_android.write(msg, 3);
			b2 = b;
		}

		b = digitalRead(BUTTON3);
		if (b != b3) {
			msg[1] = 2;
			msg[2] = b ? 0 : 1;
			usb_android.write(msg, 3);
			b3 = b;
		}

		b = digitalRead(JOY_SWITCH);
		if (b != b4) {
			msg[1] = 4;
			msg[2] = b ? 0 : 1;
			usb_android.write(msg, 3);
			b4 = b;
		}

		switch (count++ % 0x10) {
		case 0:
			val = analogRead(TEMP_SENSOR);
			msg[0] = 0x4;
			msg[1] = val >> 8;
			msg[2] = val & 0xff;
			usb_android.write(msg, 3);
			break;

		case 0x4:
			val = analogRead(LIGHT_SENSOR);
			msg[0] = 0x5;
			msg[1] = val >> 8;
			msg[2] = val & 0xff;
			usb_android.write(msg, 3);
			break;

		}
	
  }   
  
  if(!usb_android.isConnected()) conn = false;    
}

/* Test USB connectivity */
bool usbcheck()
{
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
  return( 0 );
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
