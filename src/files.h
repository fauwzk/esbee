#ifndef FILES_H
#define FILES_H

class Files
{
public:
    // Member functions
    static void appendFile(String fichier, String ajout);
    static void createFile(String date);
    static void readCurrFile();
    static void makeAveragefromfile();
    static void removeallfiles();
    static void listAllFiles();
    static bool removeFile(String path);
    static void removeAllFiles();
    static int todayFile();
    static int initFileSystem();
    static void setPath(String p);
};

#endif // FILES_H