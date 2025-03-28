

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h> // Include the WebServer library
#include <OneWire.h>
#include <DallasTemperature.h>
#include "LittleFS.h"
#include <ArduinoJson.h>
#include <NTPClient.h>

ESP8266WiFiMulti wifiMulti;  // Create an instance of the ESP8266WiFiMulti class, called 'wifiMulti'
ESP8266WebServer server(80); // Create a webserver object that listens for HTTP request on port 80

void handleRoot(); // function prototypes for HTTP handlers
void handleNotFound();
void removeallfiles();
void currTemp();
void listAllFiles();
void readCurrFile();
void makeAveragefromfile();
void removeAllFiles();

#define led_pin 12

#define ONE_WIRE_BUS 14

time_t epochTime;
struct tm *ptm;
int oldDay, oldHour, oldMinutes, currDay;
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

float getTemp()
{
  sensors.requestTemperatures();
  delay(500);
  float tempC = sensors.getTempCByIndex(0);
  Serial.print("Temperature: ");
  Serial.print(tempC);
  Serial.println("°C");
  return tempC;
}

void handleRoot()
{
  server.send(200, "text/plain", "Hello world!"); // Send HTTP status 200 (Ok) and send some text to the browser/client
}

void removeallfiles()
{
  removeAllFiles();
  server.send(200, "text/plain", "All files removed!"); // Send HTTP status 200 (Ok) and send some text to the browser/client
}

void currTemp()
{

  String formattedTime = timeClient.getFormattedTime();
  Serial.print("Formatted Time: ");
  Serial.println(formattedTime);

  JsonDocument JSONData;
  // Use the object just like a javascript object or a python dictionary
  JSONData["Date"] = getDate();
  JSONData["Heure"] = formattedTime;
  JSONData["Temperature"] = String(getTemp());
  // You can add more fields
  char data[300];
  // Converts the JSON object to String and stores it in data variable
  serializeJson(JSONData, data);
  // Set status code as 200, content type as application/json and send the data
  server.send(200, "application/json", data);
}

void handleNotFound()
{
  server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}

void appendFile(String path, String ajout)
{
  File file = LittleFS.open(path, "a");
  if (!file)
  {
    Serial.println("Erreur d'ouverture du fichier");
    return;
  }

  // Ajout de nouvelles données
  file.println(ajout);

  // Fermeture du fichier
  file.close();
  Serial.println("Ajout terminé");
  delay(5000);
  file.flush();
  file.close();
}

int getDay()
{
  return ptm->tm_mday;
}

int getHour()
{
  return ptm->tm_hour;
}

int getMinutes()
{
  return ptm->tm_min;
}

String getDate()
{
  int monthDay = getDay();
  int currentMonth = ptm->tm_mon + 1;
  int currentYear = ptm->tm_year + 1900;

  return String(String(monthDay) + "-" + String(currentMonth) + "-" + String(currentYear));
}

void createFile(String date)
{
  String fileName = "/" + date + ".txt";
  Serial.println("Creating file: " + fileName);
  if (!LittleFS.exists(fileName))
  {
    File file = LittleFS.open(fileName, "w");
    if (!file)
    {
      Serial.println("Error opening file for writing");
    }
    else
    {
      Serial.println("File opened for writing");
      file.close();
      Serial.println("File closed");
    }
  }
}

// lecture d'un fichier
void readCurrFile()
{
  String fileContent = "";
  String path = "/" + getDate() + ".txt";
  Serial.println("Lecture du fichier: " + path);

  File file = LittleFS.open(path, "r");
  String s = file.readString();
  file.close();
  server.send(200, "text/html", s);
}

void makeAveragefromfile()
{
  String path = "/" + getDate() + ".txt";
  File file = LittleFS.open(path, "r");
  String line;
  float sum = 0;
  int count = 0;
  while (file.available())
  {
    line = file.readStringUntil('\n');
    sum += line.toFloat();
    Serial.println("Line: " + line) + " Sum: " + String(sum);
    count++;
  }
  file.close();
  float average = sum / count;
  Serial.println("Average: " + String(average));
  server.send(200, "text/plain", "Average: " + String(average));
  removeFile(path);
  createFile(getDate());
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
  {                               // Check if file exists
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

void setup(void)
{
  Serial.begin(115200); // Start the Serial communication to send messages to the computer
  delay(10);
  Serial.println('\n');

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

  server.on("/", handleRoot);
  server.on("/temp", currTemp);
  server.on("/removeallfiles", removeallfiles);
  server.on("/listallfiles", listAllFiles);
  server.on("/readcurrfile", readCurrFile);
  server.on("/avg", makeAveragefromfile);
  server.onNotFound(handleNotFound); // When a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"

  server.begin(); // Actually start the server
  Serial.println("HTTP server started");

  pinMode(PIN_GROVE_POWER, OUTPUT);
  digitalWrite(PIN_GROVE_POWER, 1);
  pinMode(led_pin, OUTPUT);
  sensors.begin();

  timeClient.setTimeOffset(3600);
  timeClient.begin();
  timeClient.update();
  if (!LittleFS.begin())
  {
    Serial.println("Erreur lors de l'initialisation de LittleFS");
    return;
  }
  delay(1000);
  epochTime = timeClient.getEpochTime();
  ptm = gmtime((time_t *)&epochTime);
  oldDay = getDay();
  oldHour = getHour();
  oldMinutes = getMinutes();
  if (!LittleFS.exists("/" + getDate() + ".txt"))
  {
    createFile(getDate());
  }
  else
  {
    Serial.println("Today file already exists");
  }
}

int elapsedMinutes = 0;
void loop(void)
{
  server.handleClient();
  epochTime = timeClient.getEpochTime();
  ptm = gmtime((time_t *)&epochTime);
  currDay = getDay();
  if (currDay != oldDay)
  {
    createFile(getDate());
    oldDay = currDay;
  }
  if (getMinutes() != oldMinutes)
  {
    String filename = "/" + getDate() + ".txt";
    appendFile(filename, String(getTemp()));
    Serial.println("Minutes changed, file " + filename + " updated");
    oldMinutes = getMinutes();
    elapsedMinutes++;
    if (elapsedMinutes == 24)
    {
      elapsedMinutes = 0;
      Serial.println("Création de la moyenne");
      makeAveragefromfile();
    }
  }
  delay(1);
}
