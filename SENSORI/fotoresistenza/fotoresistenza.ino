#define InputAnalogico A0

void setup(){
   Serial.begin(9600); // Inizializzo la comunicazione a 9600
}

void loop(){
   int ValoreSensore = analogRead(InputAnalogico); // Inserisco il valore della lettura dell'input analogico sull'intero ValoreSensore
   Serial.println(ValoreSensore, DEC); // Scrivo sullo schermo il valore del sensore in numeri decimali
   delay(100);
}

