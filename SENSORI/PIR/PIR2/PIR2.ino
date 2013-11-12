#define PIR A0
#define LED 7

void setup()
{
  Serial.begin(9600);
  pinMode(PIR,INPUT);
  pinMode(LED,OUTPUT);  
}

void loop()
{
 if(analogRead(PIR)>=500)
  digitalWrite(LED,HIGH);   
 else
  digitalWrite(LED,LOW);
}
