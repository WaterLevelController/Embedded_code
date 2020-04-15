/*
A code to provide intereface to arduino board with a switch and app to control a light

The light should be turned on and turned off independently by the use of switch and App

*/
#include <FS.h> 

//Ported to ESP32
#ifdef ESP32
  #include <esp_wifi.h>
  #include <WiFi.h>
  #include <WiFiClient.h>
  #define ESP_getChipId()   ((uint32_t)ESP.getEfuseMac())

  #define LED_ON      HIGH
  #define LED_OFF     LOW  
#else
  #include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
  //needed for library
  #include <DNSServer.h>
  #include <ESP8266WebServer.h>  

  #define ESP_getChipId()   (ESP.getChipId())

  #define LED_ON      LOW
  #define LED_OFF     HIGH
#endif
#include <ESPAsyncWebServer.h>     //Local WebServer used to serve the configuration portal
#include <ESPAsyncWiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic

AsyncWebServer server(80);
DNSServer dns;

IPAddress staticIP50_10(192,168,0,25);
IPAddress gateway50_10(192,168,0,1);
IPAddress subnet50_10(255,255,255,0);

bool App_ip;
bool WifiConfig;
String message="not available";
 
// SSID and PW for Config Portal
String ssid = "ESP_" + String(ESP_getChipId(), HEX);
const char* password = "your_password";


void setup() 
{
  //start serial connection
  Serial.begin(9600);
  //Switch input
  pinMode(4, INPUT_PULLUP);
   //configuration input
  pinMode(13, INPUT_PULLUP);
  //App Input
  //pinMode(9, INPUT_PULLUP);
  //Light output
  pinMode(2, OUTPUT);

  AsyncWiFiManager ESP_wifiManager(&server,&dns);
    
  WiFi.disconnect();
  delay(1000);
  Serial.println("START");
  ESP_wifiManager.setSTAStaticIPConfig(staticIP50_10, gateway50_10, subnet50_10);
  //WiFi.config(staticIP50_10, gateway50_10, subnet50_10);
  //WiFi.begin("Raghu","123456789");
  ESP_wifiManager.autoConnect("AUTO", "123456789");
   server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/plain", "Hello, world");
    });
  
    server.on("/ON", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/plain", "1");
        App_ip = 1;
    });
    
    server.on("/OFF", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/plain", "0");
        App_ip = 0;
    });
    server.on("/WifiConfigOn", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/plain", "WifiConfig");
        WifiConfig = 0;
    });
    
    server.on("/WifiConfigOFF", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/plain", "WifiConfig");
        WifiConfig = 0;
    });

        // Send a GET request to <IP>/get?message=<message>
    
    server.on("/tank", HTTP_GET, [] (AsyncWebServerRequest *request) {
        
        if (request->hasParam("value")) {
            message = request->getParam("value")->value();
        } else {
            message = "No message sent";
        }
        request->send(200, "text/plain", "Hello, GET: " + message);
        Serial.println(message);
    });
    server.on("/tanklevel", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/plain", message);
        Serial.print("Tanklevel: ");
        Serial.print(message);
        Serial.print("/n");
    });
  while ((!(WiFi.status() == WL_CONNECTED)))
  {
    delay(300);
    Serial.print("..");

  }
//  Serial.println("Connected");
//  Serial.println("Your IP is");
//  Serial.println((WiFi.localIP()));
  server.begin();
}

void loop()
{
  bool Aux_a;
  bool Aux_b;
  static bool Stored_switch = 0; //This variable stores the old value of switch input
  static bool Stored_state = 0; //this variable stores the state of change in switch input
  bool Switch_ip;
 
  bool Output;
  Switch_ip = digitalRead(4);
  //App_ip = digitalRead(9);
  
  // confing on input

  if (digitalRead(13) == LOW || WifiConfig ==1) 
    {
      
      Serial.println("\nConfiguration portal requested.");
      digitalWrite(2, HIGH); // turn the LED on by making the voltage LOW to tell us we are in configuration mode.
      
      //Local intialization. Once its business is done, there is no need to keep it around
      AsyncWiFiManager ESP_wifiManager(&server,&dns);
      
      //Check if there is stored WiFi router/password credentials.
      //If not found, device will remain in configuration mode until switched off via webserver.
      WiFi.mode(WIFI_STA);
      ESP_wifiManager.setSTAStaticIPConfig(staticIP50_10, gateway50_10, subnet50_10);
      Serial.print("Opening configuration portal. ");
      if (!ESP_wifiManager.startConfigPortal("ON_Demand_AP")) 
      {
        Serial.println("Not connected to WiFi but continuing anyway.");
      } 
      else 
      {
        //if you get here you have connected to the WiFi
        Serial.println("connected...yeey :)");
        digitalWrite(2, LOW); // Turn led off as we are not in configuration mode.
        //ESP.restart();
        WifiConfig =0;
      }    
      }    
   
  /*
  if (ClientRequest == "ON") {
    App_ip = 1;
    //digitalWrite(2,HIGH);

  } else {
    App_ip = 0;
    //digitalWrite(2,LOW);

  }
  */
  // checking for the condition of when output is expected 
  //to change only using the app input
  if (Switch_ip != Stored_switch){
    Aux_a = 1;
  }
  else{
    Aux_a = Stored_state;
  }
  Stored_switch = Switch_ip;
  
  Aux_b = App_ip != Switch_ip;

  //store the state of switch input change only when 
  if (Aux_a && (!Aux_b)){
    Stored_state = 0;
  }
  else {
    Stored_state = Aux_a;
  }
  
  if (Aux_b){
    if (Aux_a){
      Output = Switch_ip;
    }
    else{
      Output = App_ip;
    }
  }
  else{
    Output = Switch_ip;
    }
  if (Output==HIGH){
    digitalWrite(2, LOW);
  }
  else{
    digitalWrite(2, HIGH);
  }
  Serial.print("Switch input = ");
  Serial.print(Switch_ip);
  Serial.print("\t App input =");
  Serial.print(App_ip);
  Serial.print("\t Output = ");
  Serial.print(Output);
  Serial.print("\t IP = ");
  Serial.print(WiFi.localIP());
  Serial.print("\n");
  /*
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println(App_ip);
  client.println("</html>");
  */
  delay(1000);
  }
