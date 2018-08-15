void SleepWatchdog() {

  CheckVoltage ();
  http.begin("http://192.168.1.11:8080/rover/wake.txt");
  httpCode = http.GET();
  delay(100);
  String httppayload = http.getString();
  delay(100);
  if (httppayload != "0") {
    return;
  }
  else {
    delay( 1 );
    WiFi.disconnect( true );
    ESP.deepSleep(7e6, WAKE_RF_DISABLED );
    delay( 1 );
  }
}


void CheckVoltage () {

  sensorValue = analogRead(A0);
  voltage = sensorValue * .0063;
  http.begin("http://192.168.1.11:8080/rover/savepost.php?payload=" + String(voltage));
  httpCode = http.GET();
  delay(100);
  String httppayload = http.getString();

}


void SleepRTC() {

  bDoneSleeping = false;

  if (ESP.rtcUserMemoryRead((uint32_t)RTCMemOffset, (uint32_t*) &sleepMemory, sizeof(sleepMemory)))
  {
    if (sleepMemory.magicNumber != MAGIC_NUMBER) // memory got corrupt or this is the first time
    {
      sleepMemory.bFirstTime = true;
      sleepMemory.magicNumber = MAGIC_NUMBER;
      sleepMemory.wakeCount = 0;
      bDoneSleeping = true;
    }
    else
    {
      sleepMemory.wakeCount += 1; // incrememnt the sleep counter
      sleepMemory.bFirstTime = false;
    }

    if (sleepMemory.wakeCount >= 15)
    {
      sleepMemory.wakeCount = 0;
      bDoneSleeping = true;
    }

    ESP.rtcUserMemoryWrite((uint32_t)RTCMemOffset, (uint32_t*) &sleepMemory, sizeof(sleepMemory)); // write the new values to memory
  }

  if (bDoneSleeping)
  {
    SleepWatchdog();
  }
  else {
    delay( 1 );
    WiFi.disconnect( true );
    ESP.deepSleep(7e6, WAKE_RF_DISABLED );
    delay( 1 );
  }
}

void BatteryLevel () {
  tSocket = millis();
   if((tSocket - lSocket) > 300) {                          
        
  sensorValue = analogRead(A0);
  voltage = sensorValue * .0063;
  SocketMSG = String(voltage);
        yield();
        webSocket.broadcastTXT(SocketMSG);
        lSocket = tSocket;                                     
        Serial.println(voltage);
        yield();
    }


}

