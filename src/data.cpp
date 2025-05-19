// Include the WebServer library
#include <OneWire.h>
#include <DallasTemperature.h>
// #include <NTPClient.h>
#include <time.h>
#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include "esbee.h"
#include "HX711.h"
#include "DHT.h"

struct tm ptm;
int oldDay, oldHour, oldMinutes, currDay, tempAvg;

OneWire oneWire(0); // Pin for OneWire
DallasTemperature sensors(&oneWire);
HX711 scale;
#define calibration_factor -13947 // This value is obtained using the SparkFun_HX711_Calibration sketch
#define PIN_WATER_SENSOR 3
#define DHTTYPE DHT11
#define DHTPIN 14
DHT dht(DHTPIN, DHTTYPE);

void Data::initSensors(int tempPin, int DOUT_SCALE, int CLK_SCALE)
{

	dht.begin();
	scale.begin(DOUT_SCALE, CLK_SCALE);
	oneWire.begin(tempPin); // Initialize OneWire on the specified pin
	sensors.begin();
	scale.set_scale(calibration_factor); // This value is obtained by using the SparkFun_HX711_Calibration sketch
	Data::weightTare();
	Data::getWeight();
}

void Data::weightTare()
{
	scale.tare();
}

int Data::getWeight()
{
	int test = scale.get_units() * 0.453592 * 1000;
	Serial.println(String(test));
	return test;
}

void Data::weightRead()
{
	String weight = String(getWeight(), 1);
	esbee_server.esbeeSendClient(200, "text/html", weight);
}

int Data::getDay()
{
	return ptm.tm_mday;
}

int Data::getHour()
{
	return ptm.tm_hour;
}

int Data::getMinutes()
{
	return ptm.tm_min;
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

float Data::getIntTemp()
{
	sensors.requestTemperatures();
	delay(500);
	float tempC = sensors.getTempCByIndex(0);
	Serial.print("Temperature: ");
	Serial.print(tempC);
	Serial.print("°C\n");
	return tempC;
}

String Data::getExterTemp()
{
	return String(dht.readTemperature());
}

String Data::getExterHum()
{
	return String(dht.readHumidity());
}

int Data::getLight()
{
	int value = analogRead(A0);
	return value;
}

int Data::getWater()
{
	return digitalRead(PIN_WATER_SENSOR);
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
	int currentMonth = ptm.tm_mon + 1;
	int currentYear = ptm.tm_year + 1900;
	return String(String(monthDay) + "-" + String(currentMonth) + "-" + String(currentYear));
}

String Data::createCurrJson()
{
	DynamicJsonDocument JSONData(512);
	char buffer[80];
	strftime(buffer, sizeof(buffer), "%H:%M:%S", &ptm);
	JSONData["Date"] = getDate();
	JSONData["Heure"] = buffer;
	JSONData["TemperatureInter"] = String(getIntTemp());
	JSONData["TemperatureExter"] = String(getExterTemp());
	JSONData["HumiditeExter"] = String(getExterHum());
	JSONData["Lumiere"] = String(getLight());
	JSONData["Pluie"] = String(getWater());
	JSONData["Poids"] = String(getWeight());
	String data;
	serializeJson(JSONData, data);
	return data;
}

void Data::currState()
{
	esbee_server.esbeeSendClient(200, "application/json", createCurrJson());
}

void Data::updateTime()
{
	getLocalTime(&ptm);
	char buffer[3];						 // Enough for "09" + null terminator
	sprintf(buffer, "%02d", ptm.tm_min); // Format: two digits, zero-padded
	ptm.tm_min = atoi(buffer);			 // Convert to int
}

void Data::initTime()
{
	const int decalage = 2;							   // la valeur dépend de votre fuseau horaire. Essayez 2 pour la France.
	configTime(decalage * 3600, 0, "fr.pool.ntp.org"); // serveurs canadiens								   //  en Europe, essayez europe.pool.ntp.org ou fr.pool.ntp.orgt timezone offset in seconds (e.g., 3600 for UTC+1)
	getLocalTime(&ptm);
}