// Include the WebServer library
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ArduinoJson.h>
#include <NTPClient.h>
#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>

#include "data.h"

#define ONE_WIRE_BUS 14

time_t epochTime;
struct tm *ptm;
int oldDay, oldHour, oldMinutes, currDay;

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
sensors.begin();

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

int Data::getDay()
{
    return ptm->tm_mday;
}

int Data::getHour()
{
    return ptm->tm_hour;
}

int Data::getMinutes()
{
    return ptm->tm_min;
}

float Data::getTemp()
{
    sensors.requestTemperatures();
    delay(500);
    float tempC = sensors.getTempCByIndex(0);
    Serial.print("Temperature: ");
    Serial.print(tempC);
    Serial.print("°C\n");
    return tempC;
}

String Data::getDate()
{
    int monthDay = getDay();
    int currentMonth = ptm->tm_mon + 1;
    int currentYear = ptm->tm_year + 1900;

    return String(String(monthDay) + "-" + String(currentMonth) + "-" + String(currentYear));
}

void Data::currTemp()
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
    // server.send(200, "application/json", data);
}