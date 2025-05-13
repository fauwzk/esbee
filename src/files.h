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

	void newDay();
	void removeallfiles();
	void listAllFiles();
	bool removeFile(String path);
	void setPath(String p);
	String todayFileName();
	String oldFile();

private:
	void removeAllFiles();
};

#endif // FILES_H