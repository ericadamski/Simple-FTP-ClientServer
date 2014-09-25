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

class Server
{
  public:
    Server();
    Server(std::string);
    ~Server();

    void Close();
    void printHelp();
    void error(const char*);
    void Listen();
    
    int Send(std::string);
    int Receive();

  private:
    const int PORT = 30000;

    struct sockaddr_in m_address;
    struct sockaddr_in m_clientAddress;

    socklen_t m_clientLength;

    char buffer[256];

    int m_connectionSocket;
    int m_acceptSocket;
    int m_send;
    int m_receive;
    int m_port;

    int createSocket();

    void zeroBuffer();
};

#endif
