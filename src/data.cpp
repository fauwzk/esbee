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
#include "esbee.h"
#include "HX711.h"

time_t epochTime;
struct tm *ptm;
int oldDay, oldHour, oldMinutes, currDay, tempAvg;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

OneWire oneWire(0); // Pin for OneWire
DallasTemperature sensors(&oneWire);
HX711 scale;

#define calibration_factor -13947 // This value is obtained using the SparkFun_HX711_Calibration sketch

void Data::initSensors(int tempPin, int DOUT_SCALE, int CLK_SCALE)
{
	scale.begin(DOUT_SCALE, CLK_SCALE);
	oneWire.begin(tempPin); // Initialize OneWire on the specified pin
	sensors.begin();
	scale.set_scale(calibration_factor); // This value is obtained by using the SparkFun_HX711_Calibration sketch
	Data::weightTare();
}

void Data::weightTare()
{ 
	scale.tare();
}

void Data::weightRead()
{
	Serial.print("Reading: ");
	Serial.print((scale.get_units() * 0.453592 * 1000), 1); // scale.get_units() returns a float
	Serial.print(" g");										// You can change this to kg but you'll need to refactor the calibration_factor
	Serial.println();
}

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

void Data::set_oldDay(int day)
{
	oldDay = day;
}
void Data::set_oldHour(int hour)
{
	oldHour = hour;
}
void Data::set_oldMinutes(int minutes)
{
	oldMinutes = minutes;
}
int Data::get_oldDay()
{
	return oldDay;
}
int Data::get_oldHour()
{
	return oldHour;
}
int Data::get_oldMinutes()
{
	return oldMinutes;
}
void Data::update_oldDay()
{
	oldDay = getDay();
}
void Data::update_oldHour()
{
	oldHour = getHour();
}
void Data::update_oldMinutes()
{
	oldMinutes = getMinutes();
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

void Data::setAvg(int avg)
{
	tempAvg = avg;
}

float Data::getAvg()
{
	return tempAvg;
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
	JSONData["Date"] = getDate();
	JSONData["Heure"] = formattedTime;
	JSONData["Temperature"] = String(getTemp());
	char data[300];
	serializeJson(JSONData, data);
	esbee_server.esbeeSendClient(200, "application/json", data);
}

void Data::updateTime()
{
	timeClient.update();
	epochTime = timeClient.getEpochTime();
	ptm = localtime(&epochTime);
}

void Data::initTime()
{
	timeClient.begin();
	timeClient.setTimeOffset(3600); // Set timezone offset in seconds (e.g., 3600 for UTC+1)
	timeClient.update();
	epochTime = timeClient.getEpochTime();
	ptm = localtime(&epochTime);
}