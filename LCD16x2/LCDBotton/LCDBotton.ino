#include <SoftwareSerial.h>

SoftwareSerial lcd(2, 3);

int counter = 0;
bool premuto = false;
bool old_premuto = true;

void setup(){
  Serial.begin(9600);
  lcd.begin(9600);
  pinMode(7, OUTPUT);
  pinMode(8, INPUT);
  digitalWrite(7, HIGH);
}

void loop()
{
  if(digitalRead(8)==0){ 
    delay(10);
    
    if((digitalRead(8)==0) && counter%2==0){
       digitalWrite(7,HIGH);       
    }
    if((digitalRead(8)==0) && counter%2!=0){
       digitalWrite(7,LOW);       
    }
    counter++;
    while(digitalRead(8)==0);
  }
  
  //lcd.print("$CLEAR\r\n");   //clear screen
  
  lcd.print("$GO 2 1\r\n");  // display address is : the line 2,the row 4
  lcd.print("$PRINT TIME: \r\n");  // display the character "Hello World!"
  
  lcd.print("$GO 2 7\r\n");  // display address is : the line 1,the row 1
  lcd.print("$PRINT ");
  lcd.print(millis()/1000);
  lcd.print("\r\n");  // display the character "Emartee  COM"
  
  lcd.print("$GO 1 1\r\n");  // display address is : the line 2,the row 4
  lcd.print("$PRINT COUNT: \r\n");  // display the character "Hello World!"
  
  lcd.print("$GO 1 8\r\n");  // display address is : the line 1,the row 1
  lcd.print("$PRINT ");
  lcd.print(counter);
  lcd.print("\r\n");  // display the character "Emartee  COM"
  
  lcd.print("$CURSOR 1 1\r\n");  // CURSOR move to line 1 , row 1 
  

}
