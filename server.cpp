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
}

void Server::printHelp()
{
  //TODO
}

int Server::Send(std::string msg)
{
  zeroBuffer();
  if( sizeof(msg) > 255 )
  {
    fprintf(stderr, "ERROR : Message cannot send. Message is too long.");
    return -1;
  }
  strcpy(buffer, msg.c_str());
  m_send = write(m_connectionSocket, buffer, sizeof(buffer));
  if( m_send < 0 )
  {
    fprintf(stderr, "Message send failure.");
    return -1;
  }
  return 0;
}

int Server::Receive()
{
  zeroBuffer();
  m_receive = read(m_connectionSocket, buffer, 255);
  if( m_receive < 0 )
  {
    fprintf(stderr, "ERROR reading message.");
    return -1;
  }
  return 0;
}

void Server::zeroBuffer()
{ bzero(buffer, 256); }

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
    error("ERROR on binding");
  else
  {
    listen(m_connectionSocket, 5);
    printf("Server is now listening on port %d.\n Type [quit] to shutdown server.", m_port);
    while(std::cin)
    {
      std::string line;
      std::getline(std::cin, line);

      if( !line.empty() )
      {
        if(line == "quit")
          break;
        Receive();
        Send(line.c_str());
      }
    }
  }
  Close();
}
