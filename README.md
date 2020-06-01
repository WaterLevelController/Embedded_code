# Embedded_code
This code contains the logic to server and client of an waterlevel controller

steps to create a server ESP32: (Light_Control_Async_WifiMangaer_2)
1. Setup the ESP32 board as mentioned in this link[https://www.electronics-lab.com/project/getting-started-esp32/]
2. Install ESPAsyncWifiManager,ESP wifi libraries for ESP32
3. choose the appropriate port and board for the IDE environment
4. Flash the code
5. For initial Wifi conviguration, power up the ESP32 board and wait for Wifi hotspot named 'AUTO' to appear
6. Connect to this wifi with Password : 123456789 (SSID and Password cn be changed in the code)
7. Navigate to browser when prompted for signin. Provide the password for the required network
7. This creates a server with static ip :192.168.0.25 (change this according to your network)
8. 192.168.0.25/tanklevel serves as the data recieve point from client
9. Not: to reconfigure the Wifi SSID and Password, connect pin 13 to ground


Steps to create a ESP8266 client: (WifiClient_unltrasonic.ino)

1.setup esp8266 as mentioned in this link[https://makecademy.com/getting-started-esp8266/]
2. Install wifi libraries for ESP8266
3. set SSID and password for wifi network to be connected in the code 
4. Mention the tank details in the place provided 
5. Set the ip address of the server
6. Connect the trigger pin to terminal 4 and echo pin to terminal 3
7. Values will be sent to server on 192.168.0.25/tanklevel http post
