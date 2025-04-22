#ifndef FILES_H
#define FILES_H

class Files
{
public:
    // Member functions
    int appendFile(String fichier, String ajout);
    int createFile(String date);
    int readCurrFile();
    int makeAveragefromfile();
    int todayFile();
    int initFileSystem();

    void removeallfiles();
    void listAllFiles();
    bool removeFile(String path);
    void removeAllFiles();
    void setPath(String p);
    String todayFileName();
};

#endif // FILES_H