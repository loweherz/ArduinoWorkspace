#include <SoftwareSerial.h>

#define ENQ 0x45
#define STX 0x3C
#define ETB 0x2F
#define EOT 0x3E
#define ACK 0x41
#define cNACK 0x4E


//strStato_Generale = (ENQ + "0" + STX + "RSG" + EOT);
//strSet_Temperatura = (ENQ + "0" + STX + "CST" + ETB + "200" + EOT);
//strDisabilita_Luci = (ENQ + "0" + STX + "CDL" + EOT);
//strAbilita_Luci = (ENQ + "0" + STX + "CEL" + EOT);
//strAbilita_Temp = (ENQ + "0" + STX + "CAT" + EOT);
//strDisabilita_Temp = (ENQ + "0" + STX + "CDT" + EOT);
//rispStato_Generale = (ACK + "0" + STX + "RSG" + ETB + "10101010" + ETB + "200" + EOT);
//NACK = (cNACK + "0" + EOT);
    
SoftwareSerial commPIC(2, 3);

void setup(){
    Serial.begin(9600);
    commPIC.begin(2400);
    Serial.flush();
    commPIC.flush();
    Serial.println("PROVA COMUNICAZIONE PIC");
}

void loop(){    
    accendi_tutte_luci();
    delay(5000);
    spegni_tutte_luci();
    delay(5000);
}

//                               STANZA      VALORE
//{ENQ, "0" , STX , "CAL" , ETB , "0" , ETB , "0" , EOT};
void accendi_tutte_luci(){
  for(int i=0;i<8;i++){
    commPIC.flush();
    commPIC.write((char)ENQ);
    commPIC.write('0');
    commPIC.write((char)STX);
    commPIC.write("CAL");
    commPIC.write((char)ETB);
    commPIC.write('3');
    commPIC.write((char)ETB);
    commPIC.write('1');
    commPIC.write((char)EOT);
    delay(200);
  }
}

void spegni_tutte_luci(){
  for(int i=0;i<8;i++){
    Serial.flush();
    Serial.print((char)ENQ);
    Serial.print('0');
    Serial.print((char)STX);
    Serial.print("CAL");
    Serial.print((char)ETB);
    Serial.print('3');
    Serial.print((char)ETB);
    Serial.print('0');
    Serial.println((char)EOT);
    delay(100);
  }
}
