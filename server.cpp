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

int Server::Send(struct Header msg)
{
  m_send = send(m_acceptSocket, &msg, msg.size, 0);
  if( m_send < 0 )
  {
    fprintf(stderr, "Message send failure.");
    char buffer[256];
    printf("%s",strerror_r(errno, buffer, 256));
    return -1;
  }
  printf("Message sent. \n");
  return m_send;
}

int Server::Send()
{
  m_send = send(m_acceptSocket, m_buffer, strlen(m_buffer) + 1, 0);
  printf("%d", strlen(m_buffer));
  if( m_send < 0 )
  {
    printf("%d", m_send);
    fprintf(stderr, "Message send failure.");
    return -1;
  }
  printf("Message sent. \n");
  return m_send;
}

int Server::Receive(int size)
{
  m_receive = recv(m_acceptSocket, &m_currentMsg, size, 0);
  int msgSize = ntohl(m_currentMsg.size) - sizeof(Header);
  m_buffer = (char *)malloc(msgSize);
  m_receive = recv(m_acceptSocket, m_buffer, msgSize, 0);
  if( m_receive < 0 )
  {
    fprintf(stderr, "ERROR reading message. \n");
    return -1;
  }
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

    sendResponse();
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
  //get header
  Receive(sizeof(Header));
  //handle command
  switch(m_currentMsg.msgId)
  {
    case MsgID::Type::LS:
      return handleLsCmd();
    case MsgID::Type::GET:
      return handleGetCmd();
    case MsgID::Type::PUT:
      return handlePutCmd();
    case MsgID::Type::HELP:
      printHelp();
      return 0;
    default:
      return -1;
  }
}

int Server::handleLsCmd()
{
  FILE *lsProc = popen(LS_COMMAND, "r");
  char buff[2024];
  zeroBuffer(buff);
  while(!feof(lsProc) && fgets(buff, sizeof(buff), lsProc));
  m_buffer = (char *)malloc(strlen(buff));
  strcpy(m_buffer, buff);
  struct Header msg;
  msg.msgId = MsgID::Type::LS;
  msg.size = htonl(strlen(m_buffer));
  Send(msg);
  Send(msg);
  return Send(msg);
  //return Send();
}

int Server::handleGetCmd()
{
  return 0;
}

int Server::handlePutCmd()
{
  return 0;
}
