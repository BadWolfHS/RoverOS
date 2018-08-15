
void startWebSocket() { // Start a WebSocket server
  webSocket.begin();                          // start the websocket server
  webSocket.onEvent(webSocketEvent);          // if there's an incomming websocket message, go to function 'webSocketEvent'
  Serial.println("WebSocket server started.");
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) { // When a WebSocket message is received
  switch (type) {
    case WStype_DISCONNECTED:             // if the websocket is disconnected
      Serial.printf("[%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED: {              // if a new websocket connection is established
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
      }
      break;
    case WStype_TEXT: {                    // if new text data is received
        Serial.printf("[%u] get Text: %s\n", num, payload);
        WebSocketData = atoi((const char*)&payload[1]);
        WebSocketCommand = char(payload[0]);
        Serial.println(WebSocketCommand);
        Serial.println(WebSocketData);

        if (payload[0] == 'R') {
          RPM = WebSocketData;
          Serial.println(RPM);
        }
        if (payload[0] == 'S') {
          Deg = WebSocketData;
          Serial.println(Deg);
        }
        if (payload[0] == 'D') {
          Direction = WebSocketData;
          Steps = 10000;
          Serial.println(Direction);
        }
        if (payload[0] == 'N') {
          Steps = 0;
          SteppersOff ();
          Serial.println("Stopping!");
        }
        if (payload[0] == 'C') {
          CameraPower = WebSocketData-1;
          digitalWrite(CameraPin, CameraPower);
          Serial.println(CameraPower);
        }
        if (payload[0] == 'L') {
          LEDPower = WebSocketData-1;
          digitalWrite(LEDPin, LEDPower);
          Serial.println(LEDPower);
        }
        if (payload[0] == 'F') {
          FullStep = WebSocketData;
          Serial.println(WebSocketData);
        }



      }
      //break;
  }
}
