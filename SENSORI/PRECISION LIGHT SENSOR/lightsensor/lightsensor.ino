#define InputAnalogico A0

void setup(){
   Serial.begin(9600); // Inizializzo la comunicazione a 9600
}

void loop(){
   int luce = analogRead(InputAnalogico); // Inserisco il valore della lettura dell'input analogico sull'intero ValoreSensore
   luce = (5.0* luce * 200.0) / 1024.0;
   Serial.println(luce); // Scrivo sullo schermo il valore del sensore in numeri decimali
   delay(100);
}

