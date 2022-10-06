
/* 
 
░█████╗░██████╗░███████╗░█████╗░████████╗██╗██╗░░░██╗███████╗░█████╗░███████╗███╗░░██╗████████╗██████╗░░█████╗░██╗░░░░░
██╔══██╗██╔══██╗██╔════╝██╔══██╗╚══██╔══╝██║██║░░░██║██╔════╝██╔══██╗██╔════╝████╗░██║╚══██╔══╝██╔══██╗██╔══██╗██║░░░░░
██║░░╚═╝██████╔╝█████╗░░███████║░░░██║░░░██║╚██╗░██╔╝█████╗░░██║░░╚═╝█████╗░░██╔██╗██║░░░██║░░░██████╔╝███████║██║░░░░░
██║░░██╗██╔══██╗██╔══╝░░██╔══██║░░░██║░░░██║░╚████╔╝░██╔══╝░░██║░░██╗██╔══╝░░██║╚████║░░░██║░░░██╔══██╗██╔══██║██║░░░░░
╚█████╔╝██║░░██║███████╗██║░░██║░░░██║░░░██║░░╚██╔╝░░███████╗╚█████╔╝███████╗██║░╚███║░░░██║░░░██║░░██║██║░░██║███████╗
░╚════╝░╚═╝░░╚═╝╚══════╝╚═╝░░╚═╝░░░╚═╝░░░╚═╝░░░╚═╝░░░╚══════╝░╚════╝░╚══════╝╚═╝░░╚══╝░░░╚═╝░░░╚═╝░░╚═╝╚═╝░░╚═╝╚══════╝


 *  Send a String to Websocket in Esp8266 with auto reconnect when connection got closed. 
 *  For more info contact saiareti@creativecentral.in
 
*/



#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ArduinoWebsockets.h>



// WebSocket Host and Port
const char* websockets_host = "127.0.0.1";          // Enter your websocket ip
const uint16_t websockets_port = 8080;              // Enter your websocket port
String websockets_path = "/path";                   // Your Websocket Path. Leave empty ("") if you haven't any


// WIFI Credentials
const char *ssid = "YOURSSID";                     // Enter your wifi ssid
const char *password = "YOUR_PASSWORD";            // Enter your wifi password


// Initialize Wifi and Websocket clients
using namespace websockets;
WebsocketsClient webSocket;
WiFiClient client;



// Setup
void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Receiver Host");

  Wifi();                // Initialize Wifi connection

  Websocketconnect();    // Initialize WebSocket Connection
}



// Loop Begins
void loop() {
  
  String Data = "Hello World";     // Your String may be a variable (Ex. Some Sensor data)

  Serial.println(Data);
  
  SocketState();                  // Websocket connection checker
  
  webSocket.send(Data);           // Sending a String to Websocket

  delay(1000);
  
}




// Wifi Begin
void Wifi() {
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}



// Initialize WebSocket Connection
void Websocketconnect() {
  bool connected = webSocket.connect(websockets_host, websockets_port, websockets_path);
  while (connected == false) {
    Serial.println("Connection Failed");
    delay(2000);
    Serial.println("Retrying Connection..");
    connected = webSocket.connect(websockets_host, websockets_port, websockets_path);
  }
  Serial.println("Socket connected");
}


// Websocket connection checker
void SocketState() {
  while (webSocket.available() == false) {
    Serial.println("Connection Closed");
    delay(2000);
    Serial.println("Retrying Connection..");
    Wifireconnect(); // Wifi checker
    webSocket.connect(websockets_host, websockets_port, websockets_path);
  }
  Serial.println("Connection Active");
}


// Wifi Reconnect
void Wifireconnect() {
  if (WiFi.status() != WL_CONNECTED) {
    Wifi();
  }
}
