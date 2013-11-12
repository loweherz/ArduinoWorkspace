


void setup(){
  size(700,400);
  }

void draw(){
  smooth();

  background(255);
  noStroke();
  fill(255,0,0);  //mi colora gli oggetti successivi di questo colore
  rect(20,30,200,60);
    
  
  fill(0); 
  text("apri/chiudi cancello",21,50);
  text("apri/chiudi garage",21,109);//se scrivo 100 invece di 109 ho la scritta, con 109 no
  text("status",500,20);
  
  fill(255,0,0); 
  rect(20,100,200,60);

  rect(350,50,300,300);
  rect(20,170,200,150);
  
}
