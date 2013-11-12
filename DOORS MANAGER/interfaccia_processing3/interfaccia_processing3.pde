/*VARIABILI PER LA COMUNICAZIONE*/
/*{CANCELLO, GARAGE, LED1, LED2, LED3}*/
final int PROTOCOL_LEN = 9;
final char[] apriCancello = {'p','1','0','0','0','0','0','0','f'};
final char[] chiudiCancello = {'p','0','0','0','0','0','0','0','f'};
final char[] apriGarage = {'p','0','1','0','0','0','0','0','f'};
final char[] chiudiGarage = {'p','0','0','0','0','0','0','0','f'};

char[] sysStatus = {'p','0','0','0','0','0','0','0','f'};//new char[PROTOCOL_LEN];

import processing.serial.*;

//SERIALE
Serial myPort;
char[] buffer = new char[PROTOCOL_LEN];

//GRAFICA
RectButton garageOpen, garageClose, cancelloOpen, cancelloClose;
boolean locked = false;
int ledColor = 0;

color garageOpenColor;
color garageCloseColor;
color cancelloOpenColor;
color cancelloCloseColor;

void setup(){
  size(300,300);
  
  /***INFORMAZIONI SERIALE****/
  println(Serial.list());
  String portName = "COM3";//Serial.list()[0];
  myPort = new Serial(this, portName, 9600);
  /***************************/
  
  /***GRAFICA***/  
  smooth();
  background(255);
  
  // Define and create CANCELLO OPEN button
  color cancelloOpenColor = color(255,0,0);
  color highlight = color(0); 
  cancelloOpen = new RectButton(30, 30, 100, cancelloOpenColor, highlight);

  // Define and create GARAGE OPEN button
  garageOpenColor = color(255,0,0);
  highlight = color(0); 
  garageOpen = new RectButton(30, 170, 100, garageOpenColor, highlight);

  // Define and create CANCELLO CLOSE button
  cancelloCloseColor = color(0,0,255);
  highlight = color(0); 
  cancelloClose = new RectButton(130, 30, 100, cancelloCloseColor, highlight);
  
  // Define and create GARAGE CLOSE button
  garageCloseColor = color(0,0,255);
  highlight = color(0); 
  garageClose = new RectButton(130, 170, 100, garageCloseColor, highlight);
  
  /*************/
}

//DUALE DEL LOOP in ARDUINO
void draw(){  
  update(mouseX, mouseY);  
  fill(garageOpenColor);
  garageOpen.display();
  
  fill(garageCloseColor);
  garageClose.display();
  
  fill(cancelloOpenColor);
  cancelloOpen.display();
  
  fill(cancelloCloseColor);
  cancelloClose.display();
  
  allText();
}

/***FUNZIONI PER GRAFICA***/
void update(int x, int y){
  if(locked == false) {    
    if(garageOpen.over()){
      //TODO   
    }
    else if(cancelloOpen.over()){
      //TODO
    }
    else if(garageClose.over()){
      //TODO
    }
    else if(cancelloClose.over()){
      //TODO
    }
  } 
  else {
    locked = false;
  }

  if(mousePressed) {
    if(garageOpen.pressed()) {
      //INVIO COMANDO PER GARAGE
      garageOpenColor = color(255,100,100);
      //background(0,0,255);
      sendCmd(apriCancello);
    } 
    else if(cancelloOpen.pressed()) {
      //INVIO COMANDO PER CANCELLO
      cancelloOpenColor = color(255,100,100);
      //background(255,0,0);
      sendCmd(apriGarage);
    }
    else if(garageClose.pressed()) {
      //INVIO COMANDO PER CANCELLO
      garageCloseColor = color(100,100,255);
      //background(255,0,0);
      sendCmd(chiudiGarage);
    }
    else if(cancelloClose.pressed()) {
      //INVIO COMANDO PER CANCELLO
      cancelloCloseColor = color(100,100,255);
      //background(255,0,0);
      sendCmd(chiudiCancello);
    }
  }
  else{
    garageOpenColor = color(255,0,0);
    garageCloseColor = color(0,0,255);
    cancelloOpenColor = color(255,0,0);
    cancelloCloseColor = color(0,0,255);
    
    background(255);    
  }
}

void allText(){
   int i = 0;
   fill(0); 
   text("CANCELLO",100,25);    
   text("GARAGE",100,165);
   fill(255);
   text("OPEN",60,80);   
   text("OPEN",60,230);
   text("CLOSE",160,80);
   text("CLOSE",160,230);
   fill(0); 
}

/***FUNZIONI SERIALE***/
/*void serialEvent(Serial myPort){
  int i;
  for(i=0; i<PROTOCOL_LEN; i++){
    buffer[i] = char(myPort.read());
    println(buffer[i]);
  }
  //print("\n");
  statusUpdate(buffer);
}*/

void sendCmd(char[] command){
  int i;
  for(i=0; i<PROTOCOL_LEN; i++){
    myPort.write(command[i]);
  }
  //MI SERVE DEL RITARDO PER NON INVIARE PIU COMANDI INSIEME
  delay(500);
}

/***ALTRE FUNZIONI***/
