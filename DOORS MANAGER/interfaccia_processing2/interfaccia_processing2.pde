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
RectButton garageOpen, garageClose, cancelloOpen, cancelloClose, stato;
CircleButton led1, led2, led3;
boolean locked = false;
int ledColor = 0;

void setup(){
  size(500,300);
  
  /***INFORMAZIONI SERIALE****/
  println(Serial.list());
  String portName = Serial.list()[0];
  myPort = new Serial(this, portName, 9600);
  /***************************/
  
  /***GRAFICA***/  
  smooth();
  background(255);
  
  // Define and create CANCELLO OPEN button
  color buttoncolor = color(255,0,0);
  color highlight = color(0); 
  cancelloOpen = new RectButton(20, 30, 50, buttoncolor, highlight);

  // Define and create GARAGE OPEN button
  buttoncolor = color(255,0,0);
  highlight = color(0); 
  garageOpen = new RectButton(20, 100, 50, buttoncolor, highlight);

  // Define and create CANCELLO CLOSE button
  buttoncolor = color(0,0,255);
  highlight = color(0); 
  cancelloClose = new RectButton(70, 30, 50, buttoncolor, highlight);
  
  // Define and create GARAGE CLOSE button
  buttoncolor = color(0,0,255);
  highlight = color(0); 
  garageClose = new RectButton(70, 100, 50, buttoncolor, highlight);
  
  /*
  //STATUS  
  buttoncolor = color(100,100,100);
  highlight = color(0); 
  stato = new RectButton(250, 30, 200, buttoncolor, highlight);
  
  // Define and create circle button
  buttoncolor = color(ledColor);
  highlight = color(0); 
  led1 = new CircleButton(270, 140, 20, buttoncolor, highlight);
  led2 = new CircleButton(300, 140, 20, buttoncolor, highlight);
  led3 = new CircleButton(330, 140, 20, buttoncolor, highlight);
  */
  /*************/
}

//DUALE DEL LOOP in ARDUINO
void draw(){  
  update(mouseX, mouseY);  
  garageOpen.display();
  garageClose.display();
  cancelloOpen.display();
  cancelloClose.display();
  /*
  stato.display();
  led1.display();
  led2.display();
  led3.display();
  */
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
      //background(0,0,255);
      sendCmd(apriCancello);
    } 
    else if(cancelloOpen.pressed()) {
      //INVIO COMANDO PER CANCELLO
      //background(255,0,0);
      sendCmd(apriGarage);
    }
    else if(garageClose.pressed()) {
      //INVIO COMANDO PER CANCELLO
      //background(255,0,0);
      sendCmd(chiudiGarage);
    }
    else if(cancelloClose.pressed()) {
      //INVIO COMANDO PER CANCELLO
      //background(255,0,0);
      sendCmd(chiudiCancello);
    }
  }
  else{
    background(255);    
  }
}

void allText(){
   int i = 0;
   fill(0); 
   text("CANCELLO",20,25);    
   text("GARAGE",20,95);
   text("STATO",250,25);
   /*for(i=0; i < PROTOCOL_LEN; i++){
      
      text(sysStatus[i],260+(10*i),50);
   }*/
   
   /*fill(255);
   text("I",40,60);
   text("O",90,60);
   text("I",40,130);
   text("O",90,130);*/
   
}

/***FUNZIONI SERIALE***/
void serialEvent(Serial myPort){
  int i;
  for(i=0; i<PROTOCOL_LEN; i++){
    buffer[i] = char(myPort.read());
    println(buffer[i]);
  }
  //print("\n");
  statusUpdate(buffer);
}

void sendCmd(char[] command){
  int i;
  for(i=0; i<PROTOCOL_LEN; i++){
    myPort.write(command[i]);
  }
  //MI SERVE DEL RITARDO PER NON INVIARE PIU COMANDI INSIEME
  delay(2000);
}

void statusUpdate(char[] command){
  int i;
  for(i=0; i<PROTOCOL_LEN; i++){
    sysStatus[i] = command[i];
  }
  
  //sysStatus[4] è il led1. Se 0 => ledColor = 0, else 1 => ledColor = 255;
  ledColor = ((int)sysStatus[4] - '0')*255;
}

/***ALTRE FUNZIONI***/

