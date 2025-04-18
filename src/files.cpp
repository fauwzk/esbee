#include "LittleFS.h"
#include "data.cpp"

class Files{
    public:
        String path;
        Files(String p){
            path = p;
        }

    void appendFile(String ajout)
    {
        File file = LittleFS.open(this->path, "a");
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

    void readCurrFile()
    {
        String fileContent = "";
        Serial.println("Lecture du fichier: " + this->path);

        File file = LittleFS.open(path, "r");
        String s = file.readString();
        file.close();
        //server.send(200, "text/html", s);
    }

    void makeAveragefromfile()
    {
        File file = LittleFS.open(this->path,"r");
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
        //server.send(200, "text/plain", "Average: " + String(average));
        removeFile(path);
        createFile(getDate());
    }

    void removeallfiles()
    {
        removeAllFiles();
        //server.send(200, "text/plain", "All files removed!"); // Send HTTP status 200 (Ok) and send some text to the browser/client
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
        //server.send(200, "text/plain", allFiles); // Send HTTP status 200 (Ok) and send some text to the browser/client
    }

    bool removeFile(String path)
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

    void removeAllFiles()
    {
        Dir dir = LittleFS.openDir("/"); // Open directory
        while (dir.next())
        { // List the file system contents
            Serial.print("Deleting ");
            Serial.println(dir.fileName());
            LittleFS.remove(dir.fileName());                      // Remove each file
            //server.send(200, "text/plain", "All files removed!"); // Send HTTP status 200 (Ok) and send some text to the browser/client
        }
    }
};