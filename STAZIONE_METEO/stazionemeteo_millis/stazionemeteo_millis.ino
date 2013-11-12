#include <Time.h>

#include "DHT.h"
#include <math.h>

#define DHTPIN 8     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)

#define InputSoundSensor A0
#define InputLightSensor A1
#define InputSmokeSensor A2

//TIME
unsigned long time;
unsigned long readLight_time;
unsigned long readSound_time;
unsigned long readSmoke_time;
unsigned long readDHT22_time;
  
unsigned long letturadati;
  
//VARIABLES
float Ro = 12000.0;    // this has to be tuned 12K Ohm
int sensorPin = 0;  // select the input pin for the sensor
int val = 0;        // variable to store the value coming from the sensor
float Vrl = 0.0;
float Rs = 0.0;
float ratio = 0.0;

//LED
int ledLightOK = 2;
int ledSoundOK = 3;
int ledSmokeOK = 4;
int ledDHT22OK = 5;

//SENSORI
int sound;
int light;
int smoke;
float temp;
float humid;

DHT dht(DHTPIN, DHTTYPE);

void setup(void)
{
  Serial.begin(9600);
  dht.begin();
  pinMode(ledLightOK, OUTPUT);
  pinMode(ledSoundOK, OUTPUT);
  pinMode(ledSmokeOK, OUTPUT);
  pinMode(ledDHT22OK, OUTPUT);
  
  time=millis();
  readLight_time=millis();
  readSound_time=millis();
  readSmoke_time=millis();
  readDHT22_time=millis();
  
  letturadati=millis();
}

void loop(void){ 
  time=millis(); 
  
  if(time>readSound_time+100){
    readSoundSensor();    
    readSound_time=millis();
  }
  
  if(time>readLight_time+500){
    readLightSensor();
    readLight_time=millis();
  }
  
  if(time>readSmoke_time+900){
    readSmokeSensor();
    readSmoke_time=millis();
  }
  
  if(time>readDHT22_time+2000){
    readDHT22Sensor();
    readDHT22_time=millis();
  }
  
  if(time>letturadati+3000){
      stampadati();
      letturadati=millis();
      resetLed();
   }
   
}

void readSmokeSensor(){
  val = analogRead(InputSmokeSensor);     // read the value from the analog sensor
    
  Vrl = val * ( 5.00 / 1024.0  );      // V
  Rs = 20000 * ( 5.00 - Vrl) / Vrl ;   // Ohm 
  ratio =  Rs/Ro;                      
 
  //Serial.print ( "Vrl / Rs / ratio:");
  //Serial.print (Vrl);
  //Serial.print(" ");
  //Serial.print (Rs);
  //Serial.print(" ");
  //Serial.println(ratio);
  //Serial.print ( "CO ppm :");
  //Serial.println(get_CO(ratio)); 
  smoke = get_CO(ratio);
  blinkLed(2); 
}

void readSoundSensor(){
   int value = analogRead(InputSoundSensor); // Inserisco il valore della lettura dell'input analogico sull'intero ValoreSensore
   int suono = 16.801 * log(value) + 9.872;
   //Serial.print(sound); // Scrivo sullo schermo il valore del sensore in numeri decimali
   //Serial.println(" dB");   
   blinkLed(3);
   sound = suono;
}

void readLightSensor(){
   int luce = analogRead(InputLightSensor); // Inserisco il valore della lettura dell'input analogico sull'intero ValoreSensore
   luce = (5.0* luce * 200.0) / 1024.0;
   //Serial.print(luce); // Scrivo sullo schermo il valore del sensore in numeri decimali
   //Serial.println(" Lux");
   blinkLed(4);
   light = luce;
}

void readDHT22Sensor(){
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // check if returns are valid, if they are NaN (not a number) then something went wrong!
  if (isnan(t) || isnan(h)) {
    //Serial.println("Failed to read from DHT");
    temp=0.0;
    humid=0.0;
  } else {
    //Serial.print("Humidity: "); 
    //Serial.print(h);
    //Serial.print(" %\t");
    //Serial.print("Temperature: "); 
    //Serial.print(t);
    //Serial.println(" *C");
    blinkLed(5);
    temp = t;
    humid = h;
  }  
}

// get CO ppm
float get_CO (float ratio){
  float ppm = 0.0;
  ppm = 37143 * pow (ratio, -3.178);
return ppm;
}

void blinkLed(int LED){  
    digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)    
}

void resetLed(){
    digitalWrite(2, LOW);    // turn the LED off by making the voltage LOW
    digitalWrite(3, LOW);    // turn the LED off by making the voltage LOW
    digitalWrite(4, LOW);    // turn the LED off by making the voltage LOW
    digitalWrite(5, LOW);    // turn the LED off by making the voltage LOW
}

void stampadati(){
  Serial.print("\n---SOUND SENSOR---\n");
  Serial.print(sound);
  Serial.println("dB");
  Serial.print("---LIGHT SENSOR---\n");
  Serial.print(light);
  Serial.println("Lux");
  Serial.print("---SMOKE SENSOR---\n");
  Serial.print(smoke);
  Serial.println("ppm");
  Serial.print("---DHT22 SENSOR---\n");
  if((temp==0.0)&&(humid==0.0)){
      Serial.println("ERRORE");
  }else{
      Serial.print(temp);
      Serial.print("°C - ");
      Serial.print(humid);    
      Serial.println("%");
  }
}
