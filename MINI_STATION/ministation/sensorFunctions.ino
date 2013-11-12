void readDHT11Sensor(){
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  humValue = dht.readHumidity();
  tempValue = dht.readTemperature();

  // check if returns are valid, if they are NaN (not a number) then something went wrong!
  if (isnan(tempValue) || isnan(humValue)) {
    Serial.println("Failed to read from DHT");
  } else {
    Serial.print("Humidity: "); 
    Serial.print(humValue);
    Serial.print(" %\t");
    Serial.print("Temperature: "); 
    Serial.print(tempValue);
    Serial.println(" *C");
   }  
}
