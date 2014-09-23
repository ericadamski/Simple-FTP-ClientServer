#ifndef SERVER
#define SERVER

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <list>

class Server
{
  public:
    Server();
    ~Server();

    void open();
    void close();
    void printHelp();

  private:
    std::list<int> *m_connections;

    int getNewCommunicationPort();

    void listen();
};

#endif
