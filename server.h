#ifndef SERVER
#define SERVER

#include <list>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include "messages.h"

class Server
{
  public:
    Server();
    Server(std::string);
    ~Server();

    void Close();
    void error(const char*);
    void Listen();
    
    int Send(struct Header);
    int Send();
    int Receive(int);

  private:
    const int PORT = 30000;
    const char *LS_COMMAND = "/bin/ls";

    char *m_buffer;

    struct sockaddr_in m_address;
    struct sockaddr_in m_clientAddress;

    socklen_t m_clientLength;

    struct Header m_currentMsg;

    int m_connectionSocket;
    int m_acceptSocket;
    int m_send;
    int m_receive;
    int m_port;

    int createSocket();
    int getFileSize(char*);
    int sendResponse();
    int handleGetCmd();
    int handleLsCmd();
    int handlePutCmd();

    void zeroBuffer(char*);
    void printHelp();
};

#endif
