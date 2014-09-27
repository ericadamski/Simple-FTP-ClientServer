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
    
    int Send(struct CmdResponse*);
    int Receive(int);

  private:
    const int PORT = 30000;

    struct sockaddr_in m_address;
    struct sockaddr_in m_clientAddress;

    socklen_t m_clientLength;

    char *m_buffer;

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
