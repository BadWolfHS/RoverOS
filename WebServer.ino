
//web pages

void WebPages() {

  httpServer.on("/", Root);
  httpServer.on("/Battery", HTTP_GET, httpBattery);
  httpServer.on("/Sleep", HTTP_GET, httpSleep);
  httpServer.on("/Remote", HTTP_GET, httpRemote);

}

void Root() {
  httpServer.send(200, "text/html", INDEX_HTML);
}

void httpRemote() {
  httpServer.send(200, "text/html", REMOTE_HTML);

  if (httpServer.hasArg("steps")) {
    Steps = httpServer.arg("steps").toInt();
  }

  if (httpServer.hasArg("rpm")) {
    RPM = httpServer.arg("rpm").toInt();
  }

  if (httpServer.hasArg("fullstep")) {
    FullStep = httpServer.arg("fullstep").toInt();
    StepsPerRev = 4096 / FullStep;
  }

  if (httpServer.hasArg("pan")) {
    Pan = httpServer.arg("pan").toInt();
  }

  if (httpServer.hasArg("led")) {
    LEDPower = httpServer.arg("led").toInt();
  }


  if (httpServer.hasArg("Arc")) {
    Arc = httpServer.arg("Arc").toInt();
    if (Arc > 0.1) {
      ArcL = 100 - httpServer.arg("Arc").toInt();
      ArcR = 1;
    }
    if (Arc < 0) {
      ArcR = 100 + httpServer.arg("Arc").toInt();
      ArcL = 1;
    }

    Serial.println(ArcL);
    Serial.println(ArcR);

  }

  if (httpServer.hasArg("Arc") < 0) {
    ArcR = 100 + httpServer.arg("Arc").toInt();

  }


  if (httpServer.hasArg("deg")) {
    newDeg = constrain(Deg + (httpServer.arg("deg").toInt() * Pan), 20, 160);
    Serial.println(Deg);
    Serial.println(newDeg);
  }


  if (httpServer.hasArg("direction")) {
    Direction = httpServer.arg("direction").toInt();
  }


}


void httpSleep() {
  Serial.println("Going into deep sleep");
  httpServer.send(200, "text/html", "Going to sleep<br><a href=./>Menu</a>");
  delay(500);
  ESP.deepSleep(3e6); // 20e6 is 20 microseconds
}



void httpBattery() {
  int sensorValue = analogRead(A0);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V) and adjust for the resistor divider:
  //float voltage = (sensorValue * (5.0 / 1023.0));
  float voltage = sensorValue * .0063;
  httpServer.send(200, "text/html", "Battery: " + String(voltage) + "v " + String(sensorValue)); // Send HTTP status 200 (Ok) and send some text to the browser/client
  delay(3000);

}



