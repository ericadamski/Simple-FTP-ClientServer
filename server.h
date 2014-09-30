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

    enum MsgType
    {
      MSG,
      STRING,
      OTHER
    };

    Server();
    Server(std::string);
    ~Server();

    void Close();
    void error(const char*);
    void Listen();
    
    int Send(void*, int, MsgType);
    int Receive();

  private:
    const int PORT = 30000;
    std::string LS_COMMAND = "/bin/ls";

    std::string m_data;
    std::string m_fileName;

    char *m_buffer;

    struct sockaddr_in m_address;
    struct sockaddr_in m_clientAddress;

    socklen_t m_clientLength;

    struct Header m_currentMsg;

    int ReceiveHeader();
    int ReceiveData(int);
    int m_connectionSocket;
    int m_acceptSocket;
    int m_send;
    int m_receive;
    int m_port;

    int createSocket();
    int sendResponse();
    int handleGetCmd();
    int handleLsCmd();
    int handlePutCmd();

    void zeroBuffer(char*, int);
    void printHelp();

    void *networkize(void*);
    
    Header hostize(Header);

    std::string GetStdoutFromCommand(std::string);
};

#endif
