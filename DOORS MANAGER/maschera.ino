#include <SoftwareSerial.h>

SoftwareSerial myXbee (6,7);

char enq;
int i;
char array[7];
const int m_cancello=10;
const int m_garage=11;
const int l_garage=8;
const int l_cancello=9;
const int led1=3;
const int led2=4;
const int led3=5;
const int fotor=A0;
char fine;

char cancello, garage, led_c, led_g, l1, l2, l3;

void setup(){
    Serial.begin(9600);
    pinMode(m_cancello,OUTPUT);
    pinMode(m_garage,OUTPUT);
    pinMode(l_cancello,OUTPUT);
    pinMode(l_garage,OUTPUT);
    pinMode(led1,OUTPUT);
    pinMode(led2,OUTPUT);
    pinMode(led3,OUTPUT);
    pinMode(fotor,INPUT);
    
    myXbee.begin(9600);
    myXbee.println("Xbee!");
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
		   //led_c=array[2];    
		   //led_g=array[3];    
		   //l1=array[4];    
		   //l2=array[5];    
		   //l3=array[6];    
		   
		   fine=myXbee.read();
		   
		   if(fine='f'){
			if(cancello=='0'){
				digitalWrite(m_cancello,LOW);
				}
			if(cancello=='1'){
				digitalWrite(m_cancello,HIGH);
				}
			if(garage=='0'){
				digitalWrite(m_garage,LOW);
				}
			if(garage=='1'){
			    digitalWrite(m_garage,HIGH);
				}
			/*	if(led_c=='0'){
				  digitalWrite(l_cancello,LOW);
				}
			  if(led_c=='1'){
				  digitalWrite(l_cancello,HIGH);
				}
				if(led_g=='0'){
				  digitalWrite(l_garage,LOW);
				}
			  if(led_g=='1'){
				  digitalWrite(l_garage,HIGH);
				}
				if(l1=='0'){
				  digitalWrite(led1,LOW);
				}
			  if(l1=='1'){
				  digitalWrite(led1,HIGH);
				}
				if(l2=='0'){
				  digitalWrite(led2,LOW);
				}
			  if(l2=='1'){
				  digitalWrite(led2,HIGH);
				}
				if(l3=='0'){
				  digitalWrite(led3,LOW);
				}
			  if(l3=='1'){
				  digitalWrite(led3,HIGH);
				}*/
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
}//END LOOP
            
            
            
//FUNZIONI APRI-CHIUDI GARAGE CANCELLO
