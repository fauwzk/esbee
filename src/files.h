#ifndef FILES_H
#define FILES_H

class Files
{
public:
    // Member functions
    void appendFile(String fichier, String ajout);
    void createFile(String date);
    void readCurrFile();
    void makeAveragefromfile();
    void removeallfiles();
    void listAllFiles();
    bool removeFile(String path);
    void removeAllFiles();
    int todayFile();
    int initFileSystem();
    void setPath(String p);
};

#endif // FILES_H