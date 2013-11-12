#define InputAnalogico A2

int speakerOut = 9; 

void setup() {
  Serial.begin(9600);
}

void loop() {
  int ValoreSensore = analogRead(InputAnalogico);
  if(ValoreSensore < 150)
    analogWrite(speakerOut, 200);     
  else analogWrite(speakerOut, 0);     
  Serial.println(ValoreSensore);
}
