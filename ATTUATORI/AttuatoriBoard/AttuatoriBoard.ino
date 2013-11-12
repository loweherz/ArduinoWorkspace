// Controlling a servo position using a potentiometer (variable resistor) 
// by Michal Rinott <http://people.interaction-ivrea.it/m.rinott> 

#include <Servo.h> 
 
Servo myservo;  // create servo object to control a servo 
 
int potpin = A1;  // analog pin used to connect the potentiometer
int valServo;    // variable to read the value from the analog pin 

const int buttonPin = 2;     // the number of the pushbutton pin
const int fanPin =  9;      // the number of the LED pin
 
int valFan=0; //val will be used to store state of pin 
int old_val=0;

int buttonState= 0;

void setup() 
{ 
  Serial.begin(9600);  
  // initialize the LED pin as an output:
  pinMode(fanPin, OUTPUT);      
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT); 
  
  myservo.attach(6);  // attaches the servo on pin 9 to the servo object 
} 
 
void loop() 
{ 
  valServo = analogRead(potpin);            // reads the value of the potentiometer (value between 0 and 1023) 
  valServo = map(valServo, 0, 1023, 0, 179);     // scale it to use it with the servo (value between 0 and 180) 
  myservo.write(valServo);                  // sets the servo position according to the scaled value 
  delay(15);                           // waits for the servo to get there 
  
  valFan = digitalRead(buttonPin);
  //check if input is HIGH
 
  if ((valFan == HIGH) && (old_val == LOW)) {
      buttonState = 1 - buttonState;
      delay(50);
  }
 
  old_val = valFan;
 
  if (buttonState == 1) {     
    // turn LED on:    
    digitalWrite(fanPin, HIGH);  
  } 
  else {
    // turn LED off:
    digitalWrite(fanPin, LOW); 
  }
}
