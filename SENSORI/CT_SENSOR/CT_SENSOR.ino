#include "EmonLib.h"                   // Include Emon Library
EnergyMonitor emon1;                   // Create an instance

float _power;

void readEnergy(){
  emon1.current(A1, 111.1);
  
  double Irms = emon1.calcIrms(1480);  // Calculate Irms only
  
  /*
  #ifdef SERIAL_DEBUG
    Serial.print(Irms*230.0);	       // Apparent power
    showString(PSTR(" "));
    Serial.println(Irms);		       // Irms
  #endif
  */
  _power = Irms*230.0;
}

void setup(){
  Serial.begin(9600);
}

void loop(){
  Serial.println(_power);
  Serial.println(analogRead(A1));
  Serial.println();
  delay(500);
}
