// #include "data.cpp"
#include <Arduino.h>
#include "esbee.h"

/*

SETUP

*/

Files esbee_files;
Data esbee_data;
Server esbee_server;

#define PIN_TEMPERATURE 14
#define PIN_DOUT_HX711 5
#define PIN_CLK_HX711 4
#define PIN_WATER_SENSOR 3

String filename = "";

void setup(void)
{
	Serial.begin(115200); // Start the Serial communication to send messages to the computer
	delay(100);
	esbee_server.connectWifi();
	esbee_server.startServer();
	Serial.println("HTTP server started");

	pinMode(PIN_GROVE_POWER, OUTPUT);
	pinMode(PIN_WATER_SENSOR, INPUT);
	digitalWrite(PIN_GROVE_POWER, 1);

	esbee_data.initSensors(PIN_TEMPERATURE, PIN_DOUT_HX711, PIN_CLK_HX711);
	esbee_data.initTime();
	/*
	if (esbee_files.initFileSystem() != 0)
	{
		Serial.println("Erreur lors de l'initialisation de LittleFS");
		return;
	}
		*/
	delay(1000);
	esbee_data.update_oldDay();
	esbee_data.update_oldHour();
	esbee_data.update_oldMinutes();

	// if (!LittleFS.exists("/" + data.getDate() + ".txt"))
	/*
	if (!esbee_files.todayFile())
	{
		if (esbee_files.createFile(esbee_data.getDate()) != 0)
		{
			Serial.println("Error creating file");
		}
		else
		{
			Serial.println("File created successfully");
		}
	}
	else
	{
		Serial.println("Today file already exists");
	}
	filename = esbee_files.todayFileName();
	*/
}

/*

LOOP

*/

int elapsedMinutes = 0;

void loop(void)
{
	esbee_server.esbeeHandleclient();
	esbee_data.updateTime();
	if (esbee_data.getMinutes() != esbee_data.get_oldMinutes())
	{
		esbee_server.sendDweet(esbee_data.createCurrJson());
		esbee_data.update_oldMinutes();
	}
	/*
	if (esbee_data.getDay() != esbee_data.get_oldDay())
	{
		if (esbee_files.createFile(esbee_data.getDate()) != 0)
		{
			Serial.println("Error creating file for new day");
		}
		else
		{
			Serial.println("File created successfully for new day");
		}
		esbee_data.update_oldDay();
		Serial.println("Day changed, file " + esbee_data.getDate() + ".txt created");
	}
	if (esbee_data.getMinutes() != esbee_data.get_oldMinutes())
	{

		Serial.println("Appending to file: " + filename);

		if (esbee_files.appendFile(filename, String(esbee_data.getIntTemp())) != 0)
		{
			Serial.println("Error appending to file");
			Serial.write(0x07);
		}
		else
		{
			Serial.println("Append completed");
			Serial.write(0x07);
		}

		Serial.println("Minutes changed, file " + filename + " updated");
		esbee_data.update_oldMinutes();

		elapsedMinutes++;
		if (elapsedMinutes == 24)
		{
			elapsedMinutes = 0;
			Serial.println("Création de la moyenne");
			if (esbee_files.makeAveragefromfile() != 0)
			{
				Serial.println("Erreur lors de la création de la moyenne");
				Serial.write(0x07);
			}
			else
			{
				Serial.println("Moyenne créée avec succès");
				Serial.write(0x07);
			}
			filename = esbee_files.todayFileName();
		}
	}
	*/

	/*
Send message : https://dweet.cc/dweet/for/my-thing-name?temperature=21&unit=c
curl https://dweet.cc/get/latest/dweet/for/my-thing-name
*/
	delay(1);
}
