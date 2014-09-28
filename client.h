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
#include "messages.h"

class Client
{
public:
  Client(std::string addr, std::string port);
  ~Client();
  
  void Connect();

  int Send(struct Header);
  int Receive(int);

private:
  int m_connectionSocket;
  int m_portNumber;
  int m_send;
  int m_receive;

  char *m_buffer;

  struct Header m_currentMsg;

  struct sockaddr_in m_serverAddress;
  struct hostent *m_server;

  int createSocket();
  int getFileSize(char*);
  int sendCommand(MsgID::Type, std::string);
  int sendHeader(MsgID::Type, std::string);
  int handleGetCmd();
  int handleLsCmd();
  int handlePutCmd();

  void zeroBuffer(char*, int size);
};

#endif
