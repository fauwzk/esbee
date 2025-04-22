// #include "data.cpp"
#include "server.h"
#include "files.h"
#include "data.h"

#define led_pin 12

/*

SETUP

*/

Files fichiers;
Data data;
Server server;

void setup(void)
{
  Serial.begin(115200); // Start the Serial communication to send messages to the computer
  delay(100);
  Serial.println();

  server.startServer();
  Serial.println("HTTP server started");

  pinMode(PIN_GROVE_POWER, OUTPUT);
  digitalWrite(PIN_GROVE_POWER, 1);
  pinMode(led_pin, OUTPUT);

  data.initSensors();
  data.initTime();
  if (!fichiers.initFileSystem())
  {
    Serial.println("Erreur lors de l'initialisation de LittleFS");
    return;
  }
  delay(1000);
  data.update_oldDay();
  data.update_oldHour();
  data.update_oldMinutes();

  // if (!LittleFS.exists("/" + data.getDate() + ".txt"))
  if (!fichiers.todayFile())
  {
    fichiers.createFile(data.getDate());
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
  data.updateTime();
  if (data.getDay() != data.get_oldDay())
  {
    fichiers.createFile(data.getDate());
    data.update_oldDay();
    Serial.println("Day changed, file " + data.getDate() + ".txt created");
  }
  if (data.getMinutes() != data.get_oldMinutes())
  {
    String filename = "/" + data.getDate() + ".txt";
    fichiers.appendFile(filename, String(data.getTemp()));
    Serial.println("Minutes changed, file " + filename + " updated");
    data.update_oldMinutes();
    elapsedMinutes++;
    if (elapsedMinutes == 24)
    {
      elapsedMinutes = 0;
      Serial.println("Création de la moyenne");
      fichiers.makeAveragefromfile();
    }
  }
  delay(1);
}
