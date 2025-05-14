#ifndef SERVER_H
#define SERVER_H

class Server
{
public:
	void esbeeHandleclient();
	void startServer();
	void connectWifi();
	void esbeeSendClient(int code, String contentType, String content);
	void sendDweet(String data);
};

#endif // FILES_H