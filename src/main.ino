//#include "data.cpp"
#include "server.cpp"
#include "files.cpp"

#define led_pin 12

/*

SETUP

*/
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
  // server.onNotFound(handleNotFound); // When a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"

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

/*

LOOP

*/
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
