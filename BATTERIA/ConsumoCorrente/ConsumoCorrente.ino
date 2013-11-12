const int ledPin =  7;      // the number of the LED pin
const int drawPin = A0;

int ledState = LOW;             // ledState used to set the LED
long previousMillis = 0;        // will store last time LED was updated

long interval = 1000;           // interval at which to read (milliseconds)

float voltageValue;
float resistorValue = 220.0;
float currentValue;

void setup() {
  // set the digital pin as output:
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);      
  pinMode(drawPin, INPUT);
  digitalWrite(ledPin,HIGH);
}

void loop(){
  unsigned long currentMillis = millis();
 
  if(currentMillis - previousMillis > interval) {
    // save the last time you blinked the LED 
    previousMillis = currentMillis;   

    voltageValue = analogRead(drawPin);
    currentValue = voltageValue*4.8828125/((float)resistorValue);
    Serial.print(voltageValue);
    Serial.print(" - ");
    Serial.println(currentValue);
  }
}

