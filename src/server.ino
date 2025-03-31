#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h> // Include the WebServer library
#include "LittleFS.h"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

void handleRoot();
void removeallfiles();
void listAllFiles();
bool removeFile(String path);
void removeAllFiles();

ESP8266WiFiMulti wifiMulti;  // Create an instance of the ESP8266WiFiMulti class, called 'wifiMulti'
ESP8266WebServer server(80); // Create a webserver object that listens for HTTP request on port 8

void handleRoot()
{
    server.send(200, "text/plain", "Hello world!"); // Send HTTP status 200 (Ok) and send some text to the browser/client
}

void removeallfiles()
{
    removeAllFiles();
    server.send(200, "text/plain", "All files removed!"); // Send HTTP status 200 (Ok) and send some text to the browser/client
}

void listAllFiles()
{
    String allFiles = "";
    Dir dir = LittleFS.openDir("/"); // Open directory
    while (dir.next())
    { // List the file system contents
        Serial.println("Files: ");
        Serial.println(dir.fileName());
        allFiles += dir.fileName() + "\n";
    }
    server.send(200, "text/plain", allFiles); // Send HTTP status 200 (Ok) and send some text to the browser/client
}

bool removeFile(String path)
{
    if (LittleFS.exists(path))
    {                                 // Check if file exists
        return LittleFS.remove(path); // Delete file
    }
    else
    {
        Serial.println("File not found.");
        return false;
    }
}

void removeAllFiles()
{
    Dir dir = LittleFS.openDir("/"); // Open directory
    while (dir.next())
    { // List the file system contents
        Serial.print("Deleting ");
        Serial.println(dir.fileName());
        LittleFS.remove(dir.fileName());                      // Remove each file
        server.send(200, "text/plain", "All files removed!"); // Send HTTP status 200 (Ok) and send some text to the browser/client
    }
}