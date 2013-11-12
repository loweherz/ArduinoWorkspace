#include <UsbHost.h>
#include <AndroidAccessory.h>

char accessoryName[] = "Mega_ADK"; // your Arduino board
char companyName[] = "Arduino";
bool conn = false;

MAX3421E Max;
UsbHost Usb;
AndroidAccessory usb_android(companyName, accessoryName);


void setup()
{
  usb_android.begin(); 
  Serial.begin( 9600 );
  Serial.println("Start");
  Max.powerOn();
  Serial.println("USB HOST SHIELD");  
  //if (!usbtest()) Serial.println("USB connection test failed."); 
  
  
}

void loop(){
  
  if(usb_android.isConnected() && !conn){
     conn = true;
     if (!usbcheck()) Serial.println("USB connection test failed."); 
  }    
  if(!usb_android.isConnected()){ 
    conn = false;     
  }
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
