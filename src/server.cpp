#include "esbee.h"

#include <ESP8266WiFiMulti.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h> // Include the WebServer library
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

ESP8266WiFiMulti wifiMulti;	 // Create an instance of the ESP8266WiFiMulti class, called 'wifiMulti'
ESP8266WebServer server(80); // Create a webserver object that listens for HTTP request on port 8

const char *thingName = "esbee_cressot"; // Change to something unique
const char *host = "dweet.cc";

void root()
{
	esbee_server.esbeeSendClient(200, "text/plain", "Hello world!"); // Send HTTP status 200 (Ok) and send some text to the browser/client
}

void Server::sendDweet(String data)
{
	WiFiClient client;
	const char *host = "dweet.cc";
	const int httpPort = 80;

	Serial.print("Connecting to ");
	Serial.println(host);

	if (!client.connect(host, httpPort))
	{
		Serial.println("Connection failed.");
		delay(10000);
		return;
	}
	else
	{
		Serial.println("On a gagné !");
	}
	// Construct the URL with test data
	String url = "/dweet/for/";
	url += thingName;
	url += "?temperature=" + data;

	Serial.print("Requesting URL: ");
	Serial.println(url);

	// Send HTTP GET
	client.print(String("GET ") + url + " HTTP/1.1\r\n" +
				 "Host: " + host + "\r\n" +
				 "Connection: close\r\n\r\n");

	// Wait for response
	while (client.connected())
	{
		String line = client.readStringUntil('\n');
		Serial.println(line);
		if (line == "\r")
			break;
	}

	while (client.available())
	{
		String line = client.readStringUntil('\n');
		Serial.println(line);
	}

	client.stop();
	Serial.println("Connection closed.");
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
	server.on("/now", []()
			  { esbee_data.currState(); }); // When a client requests "/temp", call the currState function
	server.on("/removeallfiles", []()
			  { esbee_files.removeallfiles(); }); // When a client requests "/removeallfiles", call the removeallfiles function
	server.on("/listallfiles", []()
			  { esbee_files.listAllFiles(); }); // When a client requests "/listallfiles", call the listAllFiles function
	server.on("/readcurrfile", []()
			  { esbee_files.readCurrFile(); }); // When a client requests "/readcurrfile", call the readCurrFile function
	server.on("/avg", []()
			  { esbee_files.makeAveragefromfile(); }); // When a client requests "/avg", call the makeAveragefromfile function
	server.on("/weight", []()
			  { esbee_data.weightRead(); }); // When a client requests "/weight", call the weightRead function
	server.begin();							 // Start the server
}