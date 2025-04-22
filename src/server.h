#ifndef SERVER_H
#define SERVER_H

class Server
{
public:
    void esbeeHandleclient();
    void startServer();
    void esbeeSendClient(int code, String contentType, String content);
};

#endif // FILES_H