#define bufferMax 128

//-- Commands and parameters (sent by browser) --
char cmd[15];    // Nothing magical about 15, increase these if you need longer commands/parameters
char param1[15];
char param2[15];

int bufferSize;
char buffer[bufferMax];

void WaitForRequest(EthernetClient client) // Sets buffer[] and bufferSize
{
  bufferSize = 0;

  while (client.connected()) {
    if (client.available()) {
	char c = client.read();
	if (c == '\n')
	  break;
	else
	  if (bufferSize < bufferMax)
	    buffer[bufferSize++] = c;
	  else
	    break;
    }
  }

  PrintNumber("bufferSize", bufferSize);
}

void ParseReceivedRequest()
{
  Serial.println("in ParseReceivedRequest");
  Serial.println(buffer);

  //Received buffer contains "GET /cmd/param1/param2 HTTP/1.1".  Break it up.
  char* slash1;
  char* slash2;
  char* slash3;
  char* space2;

  slash1 = strstr(buffer, "/") + 1; // Look for first slash
  slash2 = strstr(slash1, "/") + 1; // second slash
  slash3 = strstr(slash2, "/") + 1; // third slash
  space2 = strstr(slash2, " ") + 1; // space after second slash (in case there is no third slash)
  if (slash3 > space2) slash3=slash2;

  PrintString("slash1",slash1);
  PrintString("slash2",slash2);
  PrintString("slash3",slash3);
  PrintString("space2",space2);

  // strncpy does not automatically add terminating zero, but strncat does! So start with blank string and concatenate.
  cmd[0] = 0;
  param1[0] = 0;
  param2[0] = 0;
  strncat(cmd, slash1, slash2-slash1-1);
  strncat(param1, slash2, slash3-slash2-1);
  strncat(param2, slash3, space2-slash3-1);

  PrintString("cmd",cmd);
  PrintString("param1",param1);
  PrintString("param2",param2);
}

void PerformRequestedCommands()
{
  if ( strcmp(cmd,"digitalWrite") == 0 ) RemoteDigitalWrite();
  if ( strcmp(cmd,"digitalRead") == 0 ) RemoteDigitalRead();
  if ( strcmp(cmd,"analogRead") == 0 ) RemoteAnalogRead();
  if ( strcmp(cmd,"analogWrite") == 0 ) RemoteAnalogWrite();
  if ( strcmp(cmd,"toogleBuzzer") == 0 ) ToogleBuzzer();
}

void ToogleBuzzer(){
  digitalWrite(Buzzer, digitalRead(Buzzer)-1);
  server.print("Toogle buzzer");
}

void RemoteDigitalWrite()
{
  int ledPin = param1[0] - '0'; // Param1 should be one digit port
  int ledState = param2[0] - '0'; // Param2 should be either 1 or 0
  digitalWrite(ledPin, ledState);

  //-- Send response back to browser --
  server.print("D");
  server.print(ledPin, DEC);
  server.print(" is ");
  server.print( (ledState==1) ? "ON" : "off" );

  //-- Send debug message to serial port --
  Serial.println("RemoteDigitalWrite");
  PrintNumber("ledPin", ledPin);
  PrintNumber("ledState", ledState);
}
void RemoteDigitalRead()
{
  int ledPin = param1[0] - '0'; // Param1 should be one digit port
  int ledState; // Param2 should be either 1 or 0
  ledState = digitalRead(ledPin);

  //-- Send response back to browser --
  server.print("D");
  server.print(ledPin, DEC);
  server.print(" is ");
  server.print( (ledState==1) ? "ON" : "off" );

  //-- Send debug message to serial port --
  Serial.println("RemoteDigitalRead");
  PrintNumber("ledPin", ledPin);
  PrintNumber("ledState", ledState);
}

void RemoteAnalogRead()
{
  // If desired, use more server.print() to send http header instead of just sending the analog value.
  int analogPin = param1[0] - '0'; // Param1 should be one digit analog port
  int analogValue = analogRead(analogPin);

  //-- Send response back to browser --
  server.print("A");
  server.print(analogPin, DEC);
  server.print(" is ");
  server.print(analogValue,DEC);

  //-- Send debug message to serial port --
  Serial.println("RemoteAnalogRead");
  PrintNumber("analogPin", analogPin);
  PrintNumber("analogValue", analogValue);
}

void RemoteAnalogWrite()
{
  // If desired, use more server.print() to send http header instead of just sending the analog value.
  int analogPin = param1[0] - '0'; // Param1 should be one digit analog port
  int analogValue = param2[0] - '0';
  analogWrite(analogPin, analogValue);
  
  //-- Send response back to browser --
  server.print("A");
  server.print(analogPin, DEC);
  server.print(" is ");
  server.print(analogValue,DEC);

  //-- Send debug message to serial port --
  Serial.println("RemoteAnalogWrite");
  PrintNumber("analogPin", analogPin);
  PrintNumber("analogValue", analogValue);
}

void PrintString(char* label, char* str)
{
  Serial.print(label);
  Serial.print("=");
  Serial.println(str);
}

void PrintNumber(char* label, int number)
{
  Serial.print(label);
  Serial.print("=");
  Serial.println(number, DEC);
}
