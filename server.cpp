#include "server.h"

Server::Server()
{
  bzero((char *) &m_address, sizeof(m_address));
  m_address.sin_family = AF_INET;
  m_address.sin_addr.s_addr = INADDR_ANY;
  m_address.sin_port = htons(Server::PORT);
  Listen();
}

Server::~Server(){}

void Server::error(const char* message)
{
  perror(message);
  exit(-1);
}

int Server::createSocket()
{
  int socketId;
  if((socketId = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    error("Error creating socket.");
  return socketId;
}

void Server::Close()
{
  close(m_connectionSocket);
}

void Server::printHelp()
{
  //TODO
}

int Server::getNewCommunicationPort()
{
  //TODO
  return -1;
}

void Server::Listen()
{
  if(bind((m_connectionSocket = createSocket()),
      (struct sockaddr *) &m_address,
       sizeof(m_address)) < 0)
    error("ERROR on binding.");
  listen(m_connectionSocket, 5);
}
