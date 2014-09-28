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
    sendCommand(MsgID::Type::LS, ".");
  }
  close(m_connectionSocket);
}

int Client::createSocket()
{
  if((m_connectionSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    fprintf(stderr, "ERROR creating socket.\n");
  return m_connectionSocket;
}

int Client::Send(struct Header msg)
{
  m_send = send(m_connectionSocket, &msg, msg.size, 0);
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
  m_receive = recv(m_connectionSocket, &m_currentMsg, size, 0);
  int msgSize = ntohl(m_currentMsg.size) - sizeof(Header);
  m_buffer = (char *)malloc(msgSize);
  m_receive = recv(m_connectionSocket, m_buffer, msgSize, 0);
  printf("%d",strlen(m_buffer));
  if( m_receive < 0 )
  {
    fprintf(stderr, "ERROR receiving message.");
    return -1;
  }
  return m_receive;
}

int Client::sendCommand(MsgID::Type type, std::string command)
{
  sendHeader(type, command);
  Receive(sizeof(Header));
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
      struct LsCmd ls;
      ls.msgId = type;
      strcpy(ls.dir, command.c_str());
      ls.size = htonl(sizeof(LsCmd));
      return Send(ls);
    case MsgID::Type::GET:
      struct GetCmd get;
      get.msgId = type;
      strcpy(get.fileName, command.c_str());
      get.size = htonl(sizeof(GetCmd));
      return Send(get);
    case MsgID::Type::PUT:
      struct PutCmd put;
      put.msgId = type;
      strcpy(put.fileName, command.c_str());
      put.size = htonl(sizeof(PutCmd));
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
  printf("%s\n", m_buffer); 
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
