#include <SoftwareSerial.h>

SoftwareSerial myXbee (6,7);

unsigned int tp; //tempo passato
int p,i;
char enq;
char array[7];
const int m2_cancello=13;
const int m2_garage=12;
const int m_cancello=10;
const int m_garage=11;
const int l_garage=8;
const int l_cancello=9;
const int led1=3;
const int led2=4;
const int led3=5;
const int fotor=A0;
char fine;
int valore;

char cancello, garage, led_c, led_g, l1, l2, l3;
void apri_c();
void chiudi_c();
void apri_g();
void chiudi_g();

void setup(){
    Serial.begin(9600);
    pinMode(m_cancello,OUTPUT);
    pinMode(m2_cancello,OUTPUT);
    pinMode(m_garage,OUTPUT);
    pinMode(m2_garage,OUTPUT);
    pinMode(l_cancello,OUTPUT);
    pinMode(l_garage,OUTPUT);
    pinMode(led1,OUTPUT);
    pinMode(led2,OUTPUT);
    pinMode(led3,OUTPUT);
    pinMode(fotor,INPUT);
    
    myXbee.begin(9600);
    myXbee.println("Xbee!");
    tp=millis();
    
    for(int i=0; i<7; i++){
      array[i]='0';
    }
  }

void loop(){
    
	if(myXbee.available()>=9){
		enq=myXbee.read();
		
		if(enq=='p'){
                  Serial.println(myXbee.available());
		  Serial.println("<INFO> Comunicazione aperta");
		   for(i=0;i<7;i++){  //QUA DEVI METTERE <6 e non <10. il mio era solo un esempio
				array[i]=myXbee.read();
				Serial.print(array[i]);
		   }
		  
		   cancello=array[0];    
		   garage=array[1];    

		   fine=myXbee.read();
		   
		   if(fine='f'){
			if(cancello=='1'){
                                apri_c();
				}
                        if(cancello=='0'){
                                  chiudi_c();
				}
			if(garage=='0'){
                                chiudi_g();
				}
			if(garage=='1'){
                                chiudi_g();
				}
			} //END IF fine
			else{   
			   Serial.println("<WARNING> Comunicazione non terminata correttamente");
			}
		
		}//END IF enq
		else{
			Serial.println("<WARNING> Comunicazione non avviata");
		}
		
	}//END IF available
	
	//FUNZIONA LETTURA FOTORESISTENZA -> LED VIALETTO
            if(millis()-tp>10000){
              
            valore=analogRead(fotor);
  
              if(valore<=512){
                digitalWrite(l1,HIGH);
                digitalWrite(l2,HIGH);
                digitalWrite(l3,HIGH);
                array[4]='1';
                array[5]='1';
                array[6]='1';
              }
               else
              {
                 digitalWrite(l1,LOW);
                 digitalWrite(l2,LOW);
                 digitalWrite(l3,LOW);
                 array[4]='0';
                 array[5]='0';
                 array[6]='0';
              }
              
              
                tp=millis();
                Serial.println("mando");
                delay(10);
                myXbee.print('p');
                for(i=0;i<7;i++){
                    myXbee.print(array[i]);
                    }
                myXbee.print('f'); 
                
                }
                    
             /* for(i=0;i<6;i++){
             Serial.println(array[i]);
             }*/
}//END LOOP
            
            
            
//FUNZIONI APRI-CHIUDI GARAGE CANCELLO
void apri_c(){

      digitalWrite(m_cancello,HIGH);
      digitalWrite(m2_cancello,LOW);
      for(p=0;p<4;p++){
        digitalWrite(l_cancello,HIGH);
        delay(1000);
        digitalWrite(l_cancello,LOW);
      }
      digitalWrite(m_cancello,LOW);
      digitalWrite(m2_cancello,LOW); 
    
}

void chiudi_c(){
       
      digitalWrite(m_cancello,LOW);
      digitalWrite(m2_cancello,HIGH);
      for(p=0;p<4;p++){
        digitalWrite(l_cancello,HIGH);
        delay(1000);
        digitalWrite(l_cancello,LOW);
      }
      digitalWrite(m_cancello,LOW);
      digitalWrite(m2_cancello,LOW);
    
}

void apri_g(){
  
       digitalWrite(m_garage,HIGH);
       digitalWrite(m2_garage,LOW);
       for(p=0;p<4;p++){
          digitalWrite(l_garage,HIGH);
         delay(1000);
         digitalWrite(l_garage,LOW);
        }
        digitalWrite(m_garage,LOW);
        digitalWrite(m2_garage,LOW);
}

void chiudi_g(){
     digitalWrite(m_garage,LOW);
     digitalWrite(m2_garage,HIGH);
     for(p=0;p<4;p++){
        digitalWrite(l_garage,HIGH);
        delay(1000);
        digitalWrite(l_garage,LOW);
      }
      digitalWrite(m_garage,LOW);
      digitalWrite(m2_garage,LOW);
  }
