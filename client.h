#ifndef CLIENT
#define CLIENT

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h> 
#include <netinet/in.h>

class Client
{
public:
  Client(std::string addr, std::string port);
  ~Client();
  
  void Connect();

  int Send(std::string);
  int Receive();

private:
  int m_connectionSocket;
  int m_portNumber;
  int m_send;
  int m_receive;

  struct sockaddr_in m_serverAddress;
  struct hostent *m_server;

  char buffer[256];

  int createSocket();

  void zeroBuffer();
};

#endif
