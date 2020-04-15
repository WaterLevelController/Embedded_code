/*
    This sketch establishes a TCP connection to a "quote of the day" service.
    It sends a "hello" message, and then prints received data.
*/

#include <ESP8266WiFi.h>

#ifndef STASSID
#define STASSID "home_EXT"
#define STAPSK  "9880033538"
#endif

const char* ssid     = STASSID;
const char* password = STAPSK;

const char* host = "192.168.0.25";
const uint16_t port = 80;

WiFiClient client;
const int sleepTimeSeconds = 2;
bool Istankfull = 0;
// --------------- Tank details --------------//
const int total_height = 117; // Tank height in CM
const int hold_height = 107;// Water hold height in CM
//-------------------------------------------//

//----- minutes -----//
int minute = 1; // Data update in min.
//------------------//

const int trigger = 4;
const int echo = 5;
const int black_tank = 14; //black coloured tank
long Time;
int x;
int i;
int distanceCM;
int resultCM;
int tnk_lvl = 0;
int sensr_to_wtr = 0;

void Is_blacktankfull() {
  delay(100);
  Istankfull = digitalRead(black_tank);
}

void measure()
{
  delay(100);
  digitalWrite(trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);
  Time = pulseIn(echo, HIGH);
  distanceCM = Time * 0.034;
  resultCM = distanceCM / 2;

  tnk_lvl = map(resultCM, sensr_to_wtr, total_height, 100, 0);
  if (tnk_lvl > 100) tnk_lvl = 100;
  if (tnk_lvl < 0) tnk_lvl = 0;
}

void setup() {
  Serial.begin(9600);
  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(black_tank, INPUT_PULLUP);

  sensr_to_wtr = total_height - hold_height;

  // We start by connecting to a WiFi network
  WiFi.persistent( false );
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  int count;
  count = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    count += 1;
    if (count >= 60) {
      ESP.restart();
    }
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("connecting to ");
  Serial.print(host);
  Serial.print(':');
  Serial.println(port);

  //measure the value of tanklevel
  measure();
  Is_blacktankfull();
  // Use WiFiClient class to create TCP connections
  if (client.connect(host, port)) {
    Serial.println("connection success");
//    Serial.println("I am here");
//    Serial.print("Is tank full:");
//    Serial.println(Istankfull);
//    String url1 = "/Istankfull?value=";
//    url1 += String(random(0,100));
//    //url += String(Istankfull);
//    client.print(String("GET ") + url1 + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: keep-alive\r\n\r\n");
//    delay(100);
//    Serial.println("Response:");
//    //while (client.available()) {
//      String line1 = client.readStringUntil('\r');
//      Serial.print(line1);
//    //}
    String url = "/tank?value=";
    //url += String(random(0,100));
    url += String(tnk_lvl);
    url += "&Isblacktankfull=";
    url += String(Istankfull);
    client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: keep-alive\r\n\r\n");
    delay(100);
    Serial.println("Response:");
    //while (client.available()) {
      String line = client.readStringUntil('\r');
      Serial.print(line);
      
    //}
    delay(500);

  }
  else {
    Serial.println("connection failed");
    delay(5000);

  }


  ESP.deepSleep(1000);
  delay(1000);
  //ESP.restart();
}

void loop() {
}
