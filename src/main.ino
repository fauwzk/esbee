// #include "data.cpp"
#include "server.h"
#include "files.h"
#include "data.h"

#define led_pin 12

/*

SETUP

*/

Files fichiers;
Data donnees;
Server serveur;

void setup(void)
{
  Serial.begin(115200); // Start the Serial communication to send messages to the computer
  delay(100);
  Serial.println();

  serveur.startServer();
  Serial.println("HTTP server started");

  pinMode(PIN_GROVE_POWER, OUTPUT);
  digitalWrite(PIN_GROVE_POWER, 1);
  pinMode(led_pin, OUTPUT);

  donnees.initSensors();
  donnees.initTime();
  if (!fichiers.initFileSystem())
  {
    Serial.println("Erreur lors de l'initialisation de LittleFS");
    return;
  }
  delay(1000);
  donnees.update_oldDay();
  donnees.update_oldHour();
  donnees.update_oldMinutes();

  // if (!LittleFS.exists("/" + data.getDate() + ".txt"))
  if (!fichiers.todayFile())
  {
    fichiers.createFile(donnees.getDate());
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
  serveur.client();
  donnees.updateTime();
  if (donnees.getDay() != donnees.get_oldDay())
  {
    fichiers.createFile(donnees.getDate());
    donnees.update_oldDay();
    Serial.println("Day changed, file " + donnees.getDate() + ".txt created");
  }
  if (donnees.getMinutes() != donnees.get_oldMinutes())
  {
    String filename = "/" + donnees.getDate() + ".txt";
    fichiers.appendFile(filename, String(donnees.getTemp()));
    Serial.println("Minutes changed, file " + filename + " updated");
    donnees.update_oldMinutes();
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
