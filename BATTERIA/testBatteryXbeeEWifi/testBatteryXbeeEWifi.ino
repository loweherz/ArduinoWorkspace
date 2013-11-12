#include <avr/sleep.h>
#include <avr/wdt.h>

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#include <SoftwareSerial.h>

int batMonPin = A1;          // input pin for the divider
int val = 0;                // variable for the A/D value
float pinVoltage = 0;       // variable to hold the calculated voltage
float batteryVoltage = 0;
float batteryVin = 9.7;
float ratio = 2.1086956521739130434782608695652;        // Change this to match the MEASURED ration of the circuit
int ledBattery = 7;

int pinLed=13;

volatile boolean f_wdt=1;

long time;
long readBattery;

// initialize the library with the numbers of the interface pins
SoftwareSerial lcd(6, 7);

void setup(){
  lcd.begin(9600);
  Serial.begin(9600);
  pinMode(ledBattery,OUTPUT);
  delay(100);
  Serial.println("Setup watchdog");
  delay(2);
  // CPU Sleep Modes
  // SM2 SM1 SM0 Sleep Mode
  // 0    0  0 Idle
  // 0    0  1 ADC Noise Reduction
  // 0    1  0 Power-down
  // 0    1  1 Power-save
  // 1    0  0 Reserved
  // 1    0  1 Reserved
  // 1    1  0 Standby(1)

  cbi( SMCR,SE );	// sleep enable, power down mode
  cbi( SMCR,SM0 );     // power down mode
  sbi( SMCR,SM1 );     // power down mode
  cbi( SMCR,SM2 );     // power down mode

  setup_watchdog(9);

}

byte del;
int cnt;
int counter = 8; // timer for counting watchdog cicles

//****************************************************************
//****************************************************************
//****************************************************************
void loop(){

  if (f_wdt==1) {  // wait for timed out watchdog / flag is set when a watchdog timeout occurs
    f_wdt=0;	 // reset flag

    ///// debuging purpose only /////
     time = millis();
     Serial.print(counter);
     Serial.print(" ");
     Serial.println(time);
     delay(10); //needed for serial.print operation to finish
      ////////////////////////////////////
    checkBattery();
    if(counter==8)  // if ==10 -> this will be true every 10x8 = 80seconds; set to 225 to get 225x8=1800s = 30min
    {

	//////////////// put code inside this IF ////////

	//testWifi();
        testXbee();

	////////////////////////////////////////////////////

	counter = 0;
    }
    else counter++;

    system_sleep();

  }

}


//****************************************************************
// set system into the sleep state
// system wakes up when wtchdog is timed out
void system_sleep() {

  cbi(ADCSRA,ADEN);			  // switch Analog to Digitalconverter OFF

  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // sleep mode is set here
  sleep_enable();

  sleep_mode();				// System sleeps here

    sleep_disable();			   // System continues execution here when watchdog timed out
  sbi(ADCSRA,ADEN);			  // switch Analog to Digitalconverter ON

}

//****************************************************************
// 0=16ms, 1=32ms,2=64ms,3=128ms,4=250ms,5=500ms
// 6=1 sec,7=2 sec, 8=4 sec, 9= 8sec
void setup_watchdog(int ii) {

  byte bb;
  int ww;
  if (ii > 9 ) ii=9;
  bb=ii & 7;
  if (ii > 7) bb|= (1<<5);
  bb|= (1<<WDCE);
  ww=bb;
  Serial.println(ww);


  MCUSR &= ~(1<<WDRF);
  // start timed sequence
  WDTCSR |= (1<<WDCE) | (1<<WDE);
  // set new watchdog timeout value
  WDTCSR = bb;
  WDTCSR |= _BV(WDIE);

}
//****************************************************************
// Watchdog Interrupt Service / is executed when  watchdog timed out
ISR(WDT_vect) {
  f_wdt=1;  // set global flag 
}

void checkBattery(){
    val = analogRead(batMonPin);    // read the voltage on the divider  
    
    pinVoltage = val * 0.00488;       //  Calculate the voltage on the A/D pin
                                      //  A reading of 1 for the A/D = 0.0048mV
                                      //  if we multiply the A/D reading by 0.00488 then 
                                      //  we get the voltage on the pin.                            
    
    batteryVoltage = pinVoltage * ratio;    //  Use the ratio calculated for the voltage divider
                                            //  to calculate the battery voltage

    float percent = (batteryVoltage/batteryVin)*100;
   
    lcd.print("$CLEAR\r\n"); 
    lcd.print("$GO 1 1\r\n");  // display address is : the line 2,the row 4
    lcd.print("$PRINT Battery: \r\n");  // display the character "Hello World!"
   
    lcd.print("$GO 1 10\r\n");  // display address is : the line 1,the row 1
    lcd.print("$PRINT ");
    lcd.print(batteryVoltage);
    lcd.print("V");
    lcd.print("\r\n");  // display the character "Emartee  COM"
  
    lcd.print("$GO 2 1\r\n");
    lcd.print("$PRINT ");
    lcd.print(percent);
    lcd.print("%"); 
    lcd.print("\r\n");
    
    lcd.print("$CURSOR 1 1\r\n");
    delay(200);
    
}

void testWifi(){

}

void testXbee(){
  Serial.println("TEST XBEE");
  delay(100);
  SoftwareSerial SerialXbee(2,3);
  
  SerialXbee.print("POST /dsgm2m/api/m2m.php HTTP/1.0\r\n");
  SerialXbee.print("Content-Length: ");
  SerialXbee.print(36);
  SerialXbee.print("\r\n\r\n");
  SerialXbee.print("1234567890zxcvbnmasdfghjklqwertyuiop");
  delay(200);
}
