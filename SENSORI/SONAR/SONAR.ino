#include <SoftwareSerial.h>
SoftwareSerial rss(9, 11, true); // RX, TX

float inch1=0.00, inch2=0.00;
float cm1=0.00, cm2=0.00;

#define SonarPin A2

char *buffer;
byte x;
char array[3];
  
void setup(){
  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);
  Serial.begin(9600);
  delay(1000);  
  
  rss.begin(9600);
  delay(250);
  Serial.println("Calibrartion Cycle ");
  delay(150);
}


float readSonar_data(){    
    
  while (rss.available())
  {
     x= rss.readBytes(buffer,1);
     if(*buffer==0x52){ 
       x= rss.readBytes(buffer,1);
       array[0]=*buffer; 
       x= rss.readBytes(buffer,1);
       array[1]=*buffer; 
       x= rss.readBytes(buffer,1);
       array[2]=*buffer;
     }     
  }
  
  delayMicroseconds(200);
  
  inch1 = (array[0]-48)*100 + (array[1]-48)*10 +(array[2]-48) ;
  cm1 = inch1*2.54;
  
  int sensorValue = analogRead(SonarPin);
  delay(50);
  inch2 = (sensorValue*0.497);
  cm2 = inch2*2.54;
 
  return cm1*(0.42); 
}

float readSonar(){
  float sum = 0;
  for(int i=0; i<50; i++){
    sum+=readSonar_data();
  }
  return sum/50;
}

void loop() {
  Serial.println("\n\n");
  //float sum = 0;
  //for(int i=0; i<50; i++)
  //  sum+=readSonar();
  Serial.println(readSonar());
  delay(500);
}




