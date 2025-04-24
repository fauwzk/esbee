#include "LittleFS.h"
#include "esbee.h"

String path;

void Files::setPath(String p)
{
    path = p;
}

int Files::appendFile(String fichier, String ajout)
{
    path = Files::todayFileName();
    File file = LittleFS.open(path, "a");
    if (!file)
    {
        return 1;
    }
    // Ajout de nouvelles données
    file.println(ajout);

    // Fermeture du fichier
    file.close();
    delay(1000);
    file.flush();
    file.close();
    return 0;
}

int Files::createFile(String date)
{
    String fileName = "/" + date + ".txt";
    Serial.println("Creating file: " + fileName);
    if (!LittleFS.exists(fileName))
    {
        File file = LittleFS.open(fileName, "w");
        if (!file)
        {
            return 1;
        }
        else
        {
            file.close();
        }
    }
    return 0;
}

int Files::readCurrFile()
{
    path = Files::todayFileName();
    String fileContent = "";
    File file = LittleFS.open(path, "r");
    if (!file)
    {
        Serial.println("Failed to open file for reading");
        return 1;
    }
    String s = file.readString();
    file.close();
    esbee_server.esbeeSendClient(200, "text/html", s);
    return 0;
}

int Files::makeAveragefromfile()
{
    path = Files::todayFileName();
    File file = LittleFS.open(path, "r");
    if (!file)
    {
        return 1;
    }
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
    esbee_data.setAvg(average);
    esbee_server.esbeeSendClient(200, "text/plain", "Average: " + String(average));
    removeFile(path);
    createFile(esbee_data.getDate());
    return 0;
}

void Files::removeallfiles()
{
    removeAllFiles();
    esbee_server.esbeeSendClient(200, "text/plain", "All files removed!"); // Send HTTP status 200 (Ok) and send some text to the browser/client
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
    esbee_server.esbeeSendClient(200, "text/plain", allFiles); // Send HTTP status 200 (Ok) and send some text to the browser/client
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
        LittleFS.remove(dir.fileName());                                       // Remove each file
        esbee_server.esbeeSendClient(200, "text/plain", "All files removed!"); // Send HTTP status 200 (Ok) and send some text to the browser/client
    }
}

String Files::todayFileName()
{
    String fileName = "/" + esbee_data.getDate() + ".txt";
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