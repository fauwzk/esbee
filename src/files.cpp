#include "LittleFS.h"
#include "globals.h"

String path;

void Files::setPath(String p)
{
    path = p;
}

void Files::appendFile(String fichier, String ajout)
{
    path = Files::todayFileName();
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

void Files::createFile(String date)
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

void Files::readCurrFile()
{
    path = Files::todayFileName();
    String fileContent = "";
    Serial.println("Lecture du fichier: " + path);

    File file = LittleFS.open(path, "r");
    String s = file.readString();
    file.close();
    // serveur.esbeeSendClient(200, "text/html", s);
}

void Files::makeAveragefromfile()
{
    path = Files::todayFileName();
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
    // serveur.esbeeSendClient(200, "text/plain", "Average: " + String(average));
    removeFile(path);
    createFile(donnees.getDate());
}

void Files::removeallfiles()
{
    removeAllFiles();
    // serveur.esbeeSendClient(200, "text/plain", "All files removed!"); // Send HTTP status 200 (Ok) and send some text to the browser/client
}

void Files::listAllFiles()
{
    String allFiles = "";
    Dir dir = LittleFS.openDir("/"); // Open directory
    while (dir.next())
    { // List the file system contents
        Serial.println("Files: ");
        Serial.println(dir.fileName());
        allFiles += dir.fileName() + "\n";
    }
    // serveur.esbeeSendClient(200, "text/plain", allFiles); // Send HTTP status 200 (Ok) and send some text to the browser/client
}

bool Files::removeFile(String path)
{
    if (LittleFS.exists(path))
    {                                 // Check if file exists
        return LittleFS.remove(path); // Delete file
    }
    else
    {
        Serial.println("File not found.");
        return false;
    }
}

void Files::removeAllFiles()
{
    Dir dir = LittleFS.openDir("/"); // Open directory
    while (dir.next())
    { // List the file system contents
        Serial.print("Deleting ");
        Serial.println(dir.fileName());
        LittleFS.remove(dir.fileName()); // Remove each file
        // serveur.esbeeSendClient(200, "text/plain", "All files removed!"); // Send HTTP status 200 (Ok) and send some text to the browser/client
    }
}

String Files::todayFileName()
{
    String fileName = "/" + donnees.getDate() + ".txt";
    return fileName;
}

int Files::todayFile()
{
    String fileName = Files::todayFileName();
    Serial.println("Today file: " + fileName);
    if (LittleFS.exists(fileName))
    {
        return true;
    }
    else
    {
        return false;
    }
}

int Files::initFileSystem()
{
    if (!LittleFS.begin())
    {
        return -1;
    }
    else
    {
        return 0;
    }
}