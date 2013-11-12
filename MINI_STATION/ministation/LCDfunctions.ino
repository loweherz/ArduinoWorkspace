void printOnLCD(int page_counter){
  String row1;
  String row2;

  switch(page_counter){
    case 0:{
      row1 = "PAGINA 0";
      row2 = "Temp: ";
      row2 += tempValue;
      row2 += " C";
      if(old_temp != tempValue){
        old_temp = tempValue;
        toWrite=true;
      }
      break;
    }
    case 1:{
      row1 = "PAGINA 1";
      row2 = "Hum: ";
      row2 += humValue;
      row2 += "%";
      if(old_hum != humValue){
        old_hum = humValue;
        toWrite=true;
      }
      break;
    }
    case 2:{
      row1 = "PAGINA 2";
      row2 = "Light: ";
      break;
    }
    case 3:{
      row1 = "PAGINA 3";
      row2 = "IP: ";
      row2 += Ethernet.localIP();
      break;
    }
  }
  
  if(old_page_counter != page_counter){
      old_page_counter = page_counter;
      lcd.print("$CLEAR\r\n");   //clear screen
      Serial.println("CLEAR");
      toWrite = true;
  }
  
  if(toWrite){
      lcd.print("$GO 1 1\r\n");  
      lcd.print("$PRINT ");
      lcd.print(row1);
      lcd.print("\r\n");  
      
      lcd.print("$GO 2 1\r\n"); 
      lcd.print("$PRINT ");
      lcd.print(row2);
      lcd.print("\r\n");
      
      lcd.print("$CURSOR 1 1\r\n");  // CURSOR move to line 1 , row 1
      
      toWrite = false;
  }
}
