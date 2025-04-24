#include <ESP8266WiFiMulti.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h> // Include the WebServer library
#include "globals.h"

ESP8266WiFiMulti wifiMulti;  // Create an instance of the ESP8266WiFiMulti class, called 'wifiMulti'
ESP8266WebServer server(80); // Create a webserver object that listens for HTTP request on port 8

void root()
{
    esbee_server.esbeeSendClient(200, "text/plain", "Hello world!"); // Send HTTP status 200 (Ok) and send some text to the browser/client
}

void Server::esbeeHandleclient()
{
    server.handleClient(); // This line handles incoming client requests
}

void Server::esbeeSendClient(int code, String contentType, String content)
{
    server.send(code, contentType, content); // Send HTTP status code and content type to the client
}

void Server::connectWifi()
{
    wifiMulti.addAP("iPhone 15 Pro de Axel", "polentes"); // add Wi-Fi networks you want to connect to
    // wifiMulti.addAP("ssid_from_AP_2", "your_password_for_AP_2");
    // wifiMulti.addAP("ssid_from_AP_3", "your_password_for_AP_3");

    Serial.println("Connecting ...");
    while (wifiMulti.run() != WL_CONNECTED)
    { // Wait for the Wi-Fi to connect: scan for Wi-Fi networks, and connect to the strongest of the networks above
        Serial.print('.');
        delay(10);
    }

    Serial.println('\n');
    Serial.println("Connected to:");
    Serial.println(WiFi.SSID()); // Tell us what network we're connected to
    Serial.println("IP address:");
    Serial.println(WiFi.localIP()); // Send the IP address of the ESP8266 to the computer

    if (MDNS.begin("esbee"))
    { // Start the mDNS responder for esp8266.local
        Serial.println("mDNS responder started");
    }
    else
    {
        Serial.println("Error setting up MDNS responder!");
    }
}

void Server::startServer()
{
    server.on("/", root); // When a client requests the root URL ("/"), call the handleRoot function
    server.on("/temp", []()
              { esbee_data.currTemp(); }); // When a client requests "/temp", call the currTemp function
    server.on("/removeallfiles", []()
              { esbee_files.removeallfiles(); }); // When a client requests "/removeallfiles", call the removeallfiles function
    server.on("/listallfiles", []()
              { esbee_files.listAllFiles(); }); // When a client requests "/listallfiles", call the listAllFiles function
    server.on("/readcurrfile", []()
              { esbee_files.readCurrFile(); }); // When a client requests "/readcurrfile", call the readCurrFile function
    server.on("/avg", []()
              { esbee_files.makeAveragefromfile(); }); // When a client requests "/avg", call the makeAveragefromfile function
    server.begin();                                    // Start the server
}