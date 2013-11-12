#define ENQ 0x45
#define STX 0x3C
#define ETB 0x2F
#define EOT 0x3E
#define ACK 0x41
#define cNACK 0x4E


//strStato_Generale = (ENQ + "0" + STX + "RSG" + EOT);
//strAbilita_Temp = (ENQ + "0" + STX + "CAT" + EOT);
//strDisabilita_Temp = (ENQ + "0" + STX + "CDT" + EOT);
//rispStato_Generale = (ACK + "0" + STX + "RSG" + ETB + "10101010" + ETB + "200" + EOT);
//NACK = (cNACK + "0" + EOT);
    
void setup(){
    Serial.begin(9600);
    Serial1.begin(2400);
    Serial.flush();
    Serial1.flush();
    Serial.println("PROVA COMUNICAZIONE PIC");
    Serial1.println("PROVA COMUNICAZIONE da pic");
    Serial1.println("E0<RSG>");
    Serial1.flush();
}

void loop(){    
    accendi_temp();
    delay(5000);
    Serial1.print("E0<CAL/1/1>");
    Serial1.flush();
    //accendi_tutte_luci();
    delay(5000);
    //Serial1.println();
    //spegni_tutte_luci();
    delay(5000);
    //Serial1.println();
    //spegni_temp();
    delay(5000);
    //Serial1.println();
}

//                               STANZA      VALORE
//{ENQ, "0" , STX , "CAL" , ETB , "0" , ETB , "0" , EOT};
void accendi_tutte_luci(){
  for(int i=0;i<8;i++){
    Serial1.write((char)ENQ);
    Serial1.write('0');
    Serial1.write((char)STX);
    Serial1.write("CAL");
    Serial1.write((char)ETB);
    Serial1.print(i);
    Serial1.write((char)ETB);
    Serial1.write('1');
    Serial1.write((char)EOT);
    Serial1.flush();
    delay(500);
  }
}

void spegni_tutte_luci(){
  for(int i=0;i<8;i++){
    Serial1.write((char)ENQ);
    Serial1.write('0');
    Serial1.write((char)STX);
    Serial1.write("CAL");
    Serial1.write((char)ETB);
    Serial1.print(i);
    Serial1.write((char)ETB);
    Serial1.write('0');
    Serial1.write((char)EOT);
    Serial1.flush();
    delay(500);
  }
}

//strSet_Temperatura = (ENQ + "0" + STX + "CST" + ETB + "200" + EOT);
void accendi_temp(){  
    Serial1.write((char)ENQ);
    Serial1.write('0');
    Serial1.write((char)STX);
    Serial1.write("CST");
    Serial1.write((char)ETB);
    Serial1.write("500");
    Serial1.write((char)EOT);
    Serial1.flush();
    delay(100);  
}

void spegni_temp(){  
    Serial1.write((char)ENQ);
    Serial1.write('0');
    Serial1.write((char)STX);
    Serial1.write("CST");
    Serial1.write((char)ETB);
    Serial1.write("100");
    Serial1.write((char)EOT);
    Serial1.flush();
    delay(100);  
}

