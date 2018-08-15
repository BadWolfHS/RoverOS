#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>
#include <WebSocketsServer.h>
#include <Servo.h>


extern "C" {
#include "user_interface.h"
}

//System Configs

#define RTCMemOffset 10
#define MAGIC_NUMBER 55 // used to know if the memory is good and been written to 
int RunOnce = 0;
int sensorValue = 0;
bool bDoneSleeping;
typedef struct {
  int wakeCount;
  bool bFirstTime;
  int magicNumber;
} nvmData;
nvmData sleepMemory;

//Server Configs
const char* host = "ESP8266_1";
const char* ssid = "Jelly";
const char* password = "jellydoodle";
int WebSocketData = 0;
int WebSocketCommand = 0;
char* INDEX_HTML = "";
char* REMOTE_HTML = "";
String webString = "";
String httppayload = "";
int httpCode = 0;

//WebSocket Configs
static unsigned long lSocket = 0;                     
unsigned long tSocket;                              
int SocketPort = 81;
String SocketMSG  = "";

//Servo Configs
double ServoTick = 0;
double ServoDelay = 0;
int Pan = 10;
int Deg = 80;
int newDeg = 0;

//Stepper Configs
double StepperTick = 0;
double StepperDelay = 0;
double LastStep = 0;
//FullStep 2, HalfStep 1
double FullStep = 2;
int Steps = 0;
int StepsPerRev = 4096 / FullStep;
int Direction = 0;
int DirA = 1;
int DirB = 1;
int RPM = 4;
int Arc = 0;
int ArcR = 1;
int ArcL = 1;

//Stepper Pattern Buffer
//int w[4] = {0,0,0,0};
int w1 = 0;
int w2 = 0;
int w3 = 0;
int w4 = 0;

//Left Steppers
int motorPin1 = 14;
int motorPin2 = 12;
int motorPin3 = 13;
int motorPin4 = 15;

//Right Steppers
int motorPin5 = 5;
int motorPin6 = 4;
int motorPin7 = 0;
int motorPin8 = 2;

//Peripherals
int LEDPower = 0;
int CameraPower = 0;
int LEDPin = 1;
int CameraPin = 3;
float voltage = 0;

Servo servo;
ESP8266WebServer httpServer(80);
WebSocketsServer webSocket(81);
ESP8266HTTPUpdateServer httpUpdater;
HTTPClient http;

void setup(void) {
  delay( 1 );
  SleepRTC();

  //  WiFi.mode( WIFI_OFF );
  //  WiFi.forceSleepBegin();

  IPAddress ip( 192, 168, 1, 18);
  IPAddress gateway( 192, 168, 1, 1 );
  IPAddress subnet( 255, 255, 255, 0 );

  WiFi.forceSleepWake();
  delay( 1 );
  WiFi.persistent( false );
  WiFi.mode(WIFI_STA);
  WiFi.config( ip, gateway, subnet );

  //wifi_set_sleep_type(LIGHT_SLEEP_T);

  pinMode(LED_BUILTIN, OUTPUT);
  //Left Steppers
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
  //Right Steppers
  pinMode(motorPin5, OUTPUT);
  pinMode(motorPin6, OUTPUT);
  pinMode(motorPin7, OUTPUT);
  pinMode(motorPin8, OUTPUT);

  Serial.begin(115200);
  Serial.println();
  Serial.println("Booting RoverOS...");

  WiFi.begin(ssid, password);

  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    WiFi.begin(ssid, password);
    Serial.println("WiFi failed, retrying.");
  }

  SleepWatchdog();


  MDNS.begin(host);

  //Populate WebUI
  HTML();
  WebPages();

  httpUpdater.setup(&httpServer);
  httpServer.begin();
  startWebSocket();
  MDNS.addService("http", "tcp", 80);
  Serial.printf("HTTPUpdateServer started.\n", host);


}

void loop(void) {

  httpServer.handleClient();
  webSocket.loop();

  //Drive

  if (Steps > 0) {
    StepperRotate(Direction, RPM, Arc);
  }
  else
  {
    SteppersOff();
  }

  //Pan
  if (newDeg != Deg) {
    ServoTick = micros();
    servo.attach(3);
    servo.write(160 - constrain(Deg, 20, 140));
    Deg = newDeg;
  }
  if (ServoTick < micros() - 300000) {
    servo.detach();
  }

  BatteryLevel();
}



