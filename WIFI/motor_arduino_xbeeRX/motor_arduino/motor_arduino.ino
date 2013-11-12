//Program by Jeremy Blum
//www.jeremyblum.com
//Controls a Servo Motor using the info from an IR Distance Sensor

//Include Servo Library
#include <Servo.h>

//Define Pins
int servoPin = 9;
int oldPos = 0;

//Create Servo Object
Servo jeremysServo;

void setup()
{
 //Start Serial
 Serial.begin(9600);
  
  //Attaches the Servo to our object
  jeremysServo.attach(servoPin);
  
  delay(500);
}

void loop()
{  
  
  while( Serial.available() == 0);
  int data = Serial.read();

  //int pos = map(data, 0, 180, 0, 180);
  //pos = constrain(pos, 0, 180);

  int pos=data;
  //Turn the servo
  if(pos!=oldPos && pos!=0){
    Serial.println(pos);
    jeremysServo.write(pos);
    oldPos=pos;     
  }  
}
