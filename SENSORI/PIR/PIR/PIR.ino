// example for the PIR motion sensor SE-10
int timer = 5000;
int alarmPin = 7;
int alarmValue = 0;
int ledPin = 8;

void setup () {
    Serial.begin (9600);
    pinMode(ledPin, OUTPUT);  
    pinMode(alarmPin, INPUT);
    digitalWrite(alarmPin, HIGH);
    delay (2000); // it takes the sensor 2 seconds to scan the area around it before it can detect infrared presence. 
}

void loop (){
    alarmValue = digitalRead(alarmPin);
    Serial.println (alarmValue);
    
    if (alarmValue == LOW){
       blinky(); // blinks when the motion has been detected, just for confirmation.
    }
    delay(timer);      
}

void blinky() {
     for(int i=0; i<3; i++) {
         digitalWrite(ledPin,HIGH);
         delay(50);
         digitalWrite(ledPin,LOW);
         delay(50);
     }
}
