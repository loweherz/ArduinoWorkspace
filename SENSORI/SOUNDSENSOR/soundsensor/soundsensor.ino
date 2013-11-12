#include <math.h>
#define InputAnalogico A0

void setup(){
   Serial.begin(9600); // Inizializzo la comunicazione a 9600
}

void loop(){
   int value = analogRead(InputAnalogico); // Inserisco il valore della lettura dell'input analogico sull'intero ValoreSensore
   int sound = 16.801 * log(value) + 9.872;
   Serial.print(value); // Scrivo sullo schermo il valore del sensore in numeri decimali
   Serial.print(" - ");
   Serial.println(sound);
   delay(500);
}

