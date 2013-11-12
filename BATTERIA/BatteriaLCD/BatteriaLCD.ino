#include <LiquidCrystal.h>

int batMonPin = A1;          // input pin for the divider
int val = 0;                // variable for the A/D value
float pinVoltage = 0;       // variable to hold the calculated voltage
float batteryVoltage = 0;
float ratio = 2.1065;        // Change this to match the MEASURED ration of the circuit
int ledBattery = 7;

long time;
long readBattery;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  pinMode(ledBattery,OUTPUT);
  // set up the LCD's number of columns and rows: 
  lcd.begin(20, 4);
  // Print a message to the LCD.
  lcd.setCursor(6,0);
  lcd.print("PROVA LCD");
  lcd.setCursor(0,1);
  lcd.print("TIME: ");
  lcd.setCursor(0,2);
  lcd.print("BATTERY LV: ");  
}

void loop() {
  time = millis();
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(5, 1);
  // print the number of seconds since reset:
  lcd.print(time/1000);
  
  if(time>readBattery + 1000){
      readBattery=millis();
      checkBattery();
  }  
}

void checkBattery(){
    val = analogRead(batMonPin);    // read the voltage on the divider  
    
    pinVoltage = val * 0.00488;       //  Calculate the voltage on the A/D pin
                                      //  A reading of 1 for the A/D = 0.0048mV
                                      //  if we multiply the A/D reading by 0.00488 then 
                                      //  we get the voltage on the pin.                            
    
    batteryVoltage = pinVoltage * ratio;    //  Use the ratio calculated for the voltage divider
                                            //  to calculate the battery voltage
    lcd.setCursor(12, 2);
    lcd.print(batteryVoltage);
    lcd.print("V");
    
    if(batteryVoltage < 10) ledBattery=HIGH;
}

