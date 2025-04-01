#include <ESP8266WiFiMulti.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h> // Include the WebServer library
ESP8266WiFiMulti wifiMulti;  // Create an instance of the ESP8266WiFiMulti class, called 'wifiMulti'
ESP8266WebServer server(80); // Create a webserver object that listens for HTTP request on port 8

//void handleRoot();
void removeallfiles();
void removeAllFiles();
void listAllFiles();
bool removeFile(String path);

void handleRoot()
{
    server.send(200, "text/plain", "Hello world!"); // Send HTTP status 200 (Ok) and send some text to the browser/client
}