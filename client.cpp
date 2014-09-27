#include "client.h"

Client::Client(std::string addr, std::string port)
{
  m_portNumber = atoi(port.c_str());
  m_server = gethostbyname(addr.c_str());

  if(m_server == NULL)
    fprintf(stderr, "ERROR no such host.\n");

  bzero((char *) &m_serverAddress, sizeof(m_serverAddress));
  m_serverAddress.sin_family = AF_INET;

  bcopy((char *)m_server->h_addr,
    (char *) &m_serverAddress.sin_addr.s_addr,
    m_server->h_length);
  m_serverAddress.sin_port = htons(m_portNumber);
}

Client::~Client(){}

void Client::Connect()
{
  if(connect(createSocket(),
    (struct sockaddr *) &m_serverAddress,
    sizeof(m_serverAddress)) < 0)
    fprintf(stderr, "ERROR connecting to server.\n");
  else
  {
    printf("Type [quit] in order to close the connection.\n");
  }
  close(m_connectionSocket);
}

int Client::createSocket()
{
  if((m_connectionSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    fprintf(stderr, "ERROR creating socket.\n");
  return m_connectionSocket;
}

int Client::Send(struct Header *msg)
{
  m_send = send(m_connectionSocket, msg, sizeof(msg->size), 0);
  if( m_send < 0 )
  {
    fprintf(stderr, "Message send failure.");
    return -1;
  }
  printf(" message sent\n");
  return m_send;
}

int Client::Receive(int size)
{
  char *buffer = (char *)malloc(sizeof(size));
  zeroBuffer(buffer);
  m_receive = recv(m_connectionSocket, buffer, size, 0);
  if( m_receive < 0 )
  {
    fprintf(stderr, "ERROR receiving message.");
    return -1;
  }
  fflush(stdout);
  printf("RECEIVE : %s\n", buffer);
  m_buffer = (char *)malloc(sizeof(buffer));
  m_buffer = buffer;
  free(buffer);
  return m_receive;
}

int Client::sendCommand(MsgID::Type type, std::string command)
{
  //send header
  sendHeader(type, command);
  Receive(256);
  //struct CmdResponse *cmd = m_buffer;
  //Receive(cmd->size);
  //call specific handler
  switch(type)
  {
    case MsgID::Type::LS:
      return handleLsCmd();
    case MsgID::Type::GET:
      return handleGetCmd();
    case MsgID::Type::PUT:
      return handlePutCmd();
    case MsgID::Type::HELP:
      return 1;
    default:
      return -1;
  }
}

int Client::sendHeader(MsgID::Type type, std::string command)
{
  switch(type)
  {
    case MsgID::Type::LS:
      struct LsCmd *ls;
      ls->msgId = type;
      strcpy(ls->dir, command.c_str());
      ls->size = htonl(sizeof(int) +
                       sizeof(ls->dir) +
                       sizeof(ls->msgId));
      return Send(ls);
    case MsgID::Type::GET:
      struct GetCmd *get;
      get->msgId = type;
      strcpy(get->fileName, command.c_str());
      get->size = htonl(sizeof(int) +
                        sizeof(get->fileName) +
                        sizeof(get->msgId));
      return Send(get);
    case MsgID::Type::PUT:
      struct PutCmd *put;
      put->msgId = type;
      strcpy(put->fileName, command.c_str());
      put->size = htonl(sizeof(int) +
                        sizeof(put->fileName) +
                        sizeof(getFileSize(put->fileName)) +
                        sizeof(put->msgId));
      return Send(put);
    case MsgID::Type::HELP:
      return 1;
    default:
      return -1;
  }
}

int Client::handleGetCmd()
{
  return 0;
}

int Client::handleLsCmd()
{
  return 0;
}

int Client::handlePutCmd()
{
  return 0;
}

int Client::getFileSize(char *fileName)
{
  FILE *file = fopen(fileName, "r");
  fseek(file, 0, SEEK_END);
  return ftell(file);
}


void Client::zeroBuffer(char *buffer)
{
  bzero(buffer, sizeof(buffer));
}
