#ifndef SERVER
#define SERVER

#include <list>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

class Server
{
  public:
    Server(); //TODO: we may need to add a constructor that takes a port number as a param.
    ~Server();

    void Close();
    void printHelp();
    void error(const char*);

  private:
    std::list<int> *m_connections;

    const int PORT = 30000;

    struct sockaddr_in m_address;

    char buffer[256];

    int m_connectionSocket;
    int createSocket();
    int getNewCommunicationPort();

    void Listen();
};

#endif
