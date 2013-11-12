#include <SoftwareSerial.h>
#include <String.h>
 
SoftwareSerial mySerial(7,8);
 
void setup()
{
  mySerial.begin(19200);               // the GPRS baud rate   
  Serial.begin(19200);    // the GPRS baud rate 
  delay(500);
}
 
void loop()
{
  //after start up the program, you can using terminal to connect the serial of gprs shield,
  //if you input 't' in the terminal, the program will execute SendTextMessage(), it will show how to send a sms message,
  //if input 'd' in the terminal, it will execute DialVoiceCall(), etc.
 
  if (Serial.available())
    switch(Serial.read())
   {
     case 't':
       SendTextMessage();
       break;
     case 'd':
       DialVoiceCall();
       break;
     case 'i':
       initialSetup(1000);
       sendGSMData("mancio.myds.me","ibimet/logger.php?msg=");
       break;
     
   } 
  if (mySerial.available())
    Serial.write(mySerial.read());
}
 
///SendTextMessage()
///this function is to send a sms message
void SendTextMessage()
{
  mySerial.print("AT+CMGF=1\r");    //Because we want to send the SMS in text mode
  delay(100);
  mySerial.println("AT + CMGS = \"+393467106445\"");//send sms message, be careful need to add a country code before the cellphone number
  delay(100);
  mySerial.println("How are you ?");//the content of the message
  delay(100);
  mySerial.println((char)26);//the ASCII code of the ctrl+z is 26
  delay(100);
  mySerial.println();
}
 
///DialVoiceCall
///this function is to dial a voice call
void DialVoiceCall()
{
  mySerial.println("ATD + +393467106445;");//dial the number
  delay(100);
  mySerial.println();
}
 
void ShowSerialData()
{
  while(mySerial.available()!=0){
    char dato = mySerial.read();
    Serial.print(dato);
  }
}

void initialSetup(int initDelay){
  delay(initDelay);
  mySerial.println("AT+CGATT?");//interroga lo stato
  delay(5000);
  ShowSerialData();
  mySerial.println("AT+CSTT=\"ibox.tim.it\",\"\",\"\"");//definisci APN e ISP
  delay(7000);
  ShowSerialData();
  mySerial.println("AT+CIICR");//bring up wireless connection
  delay(7000);
  ShowSerialData();
  mySerial.println("AT+CIFSR");//get local IP adress
  delay(5000);
  ShowSerialData();
  mySerial.println("AT+CIPSPRT=0");
  delay(3000);
  ShowSerialData();
  //mySerial.end();
}

void sendGSMData(char* server, char* file){
  //DateTime now = RTC.now();
  //ciclo_wr=now.hour(); //metti ora DOPO
  
  //mySerial.begin(9600);
  mySerial.print("AT+CIPSTART=\"TCP\",\"");
  mySerial.print(server);
  mySerial.println("\",\"80\"\r");//start up the connection
  delay(1000);
  ShowSerialData();
  delay(1000);
  mySerial.println("AT+CIPSEND");//begin send data to remote server
  delay(1000);
  ShowSerialData();
  mySerial.print("GET /");
  mySerial.print(file);
  /*mySerial.print(String(now.year(), DEC)+","+String(now.month(), DEC)+","+String(now.day(), DEC)+","+String(now.hour(), DEC)+","+String(now.minute(), DEC)+",");
  mySerial.print("T9,");
  mySerial.print(n);
  mySerial.print(",");
  mySerial.print(tempsum);
  mySerial.print(",");
  mySerial.print(humsum);
  mySerial.print(",");
  mySerial.print(micsum);
  mySerial.print(",");
  mySerial.println(gprmc);
  delay(1000);	*/
  mySerial.print("AAA_1");
  mySerial.println(" HTTP/1.0");
  //mySerial.println();
  mySerial.println((char)26);//sending
  delay(5000);//waitting for reply, important! the time is base on the condition of internet 
  ShowSerialData();
  mySerial.println("AT+CIPCLOSE");//close the connection
  delay(100); 
  ShowSerialData();
  mySerial.flush();
  //mySerial.end();
}
