int batMonPin = 0;          // input pin for the divider
int val = 0;                // variable for the A/D value
float pinVoltage = 0;       // variable to hold the calculated voltage
float batteryVoltage = 0;
float ratio = 2.316;        // Change this to match the MEASURED ration of the circuit
                  
//PARTITORE DI TENSIONE
//R1 = 50k, R2 = 40k per un assorbimento di 100uA e un Vout=4V    
//RATIO = Vin/Vout

void setup() {  
    Serial.begin(9600);      // open the serial port at 9600 baud
}

void loop() {  
    val = analogRead(batMonPin);    // read the voltage on the divider  
    
    pinVoltage = val * 0.00488;       //  Calculate the voltage on the A/D pin
                                      //  A reading of 1 for the A/D = 0.0048mV
                                      //  if we multiply the A/D reading by 0.00488 then 
                                      //  we get the voltage on the pin.                            
    
    batteryVoltage = pinVoltage * ratio;    //  Use the ratio calculated for the voltage divider
                                            //  to calculate the battery voltage
    Serial.print("Voltage: ");
    Serial.println(batteryVoltage); 
    
    delay(1000);                  //  Slow it down
}
