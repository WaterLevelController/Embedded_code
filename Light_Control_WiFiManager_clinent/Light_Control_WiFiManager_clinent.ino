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
void setup() {
  Serial.begin(9600);

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

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("connecting to ");
  Serial.print(host);
  Serial.print(':');
  Serial.println(port);

  // Use WiFiClient class to create TCP connections
  if (client.connect(host, port)) {
    Serial.println("connection success");
    String url = "/tank?value=";
    url += String(random(0,100));
    client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: keep-alive\r\n\r\n");
    delay(100);
    Serial.println("Response:");
    while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
      
  }
  else{
    Serial.println("connection failed");
    delay(5000);
    return;
  }

  ESP.deepSleep(1000);
  delay(1000);
  //ESP.restart();
  }

void loop() {
  

}
