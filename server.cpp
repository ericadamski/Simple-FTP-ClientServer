#include "server.h"

Server::Server()
{
  bzero((char *) &m_address, sizeof(m_address));
  m_address.sin_family = AF_INET;
  m_address.sin_addr.s_addr = INADDR_ANY;
  m_address.sin_port = htons(PORT);
}

Server::Server(std::string port)
{
  bzero((char *) &m_address, sizeof(m_address));
  m_address.sin_family = AF_INET;
  m_address.sin_addr.s_addr = INADDR_ANY;
  m_address.sin_port =
    htons((m_port = atoi(port.c_str())));
}

Server::~Server(){}

void Server::error(const char* message)
{
  perror(message);
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
  close(m_acceptSocket);
}

void Server::printHelp()
{
  //TODO
}

int Server::Send(struct CmdResponse *msg)
{
  m_send = send(m_acceptSocket, msg, sizeof(msg->size), 0);
  if( m_send < 0 )
  {
    fprintf(stderr, "Message send failure.");
    return -1;
  }
  printf("Message sent. \n");
  return m_send;
}

int Server::Receive(int size)
{
  char *buffer = (char *)malloc(size);
  m_receive = recv(m_acceptSocket, buffer, size, 0);
  if( m_receive < 0 )
  {
    fprintf(stderr, "ERROR reading message.");
    return -1;
  }
  m_buffer = (char *)malloc(sizeof(buffer));
  m_buffer = buffer;
  return m_receive;
}

void Server::zeroBuffer(char *buffer)
{ bzero(buffer, sizeof(buffer)); }

void Server::Listen()
{
  if(bind((m_connectionSocket = createSocket()),
      (struct sockaddr *) &m_address,
       sizeof(m_address)) < 0)
    error("ERROR on binding");
  else
  {
    listen(m_connectionSocket, 5);
    printf("Server is now listening on port %d.\n Type [quit] to shutdown server.\n", m_port);
    m_clientLength = sizeof(m_clientAddress);
    m_acceptSocket = accept(m_connectionSocket,
                            (struct sockaddr *) &m_clientAddress, &m_clientLength);
    if(m_acceptSocket < 0)
    {
      fprintf(stderr, "ERROR on accept.\n");
    }
  }
  Close();
}

int Server::getFileSize(char *fileName)
{
  FILE *file = fopen(fileName, "r");
  fseek(file, 0, SEEK_END);
  return ftell(file);
} 

int Server::sendResponse()
{
  Receive(256);
  return 0;
}

int Server::handleLsCmd()
{

  return 0;
}

int Server::handleGetCmd()
{
  return 0;

}

int Server::handlePutCmd()
{
  return 0;
}
