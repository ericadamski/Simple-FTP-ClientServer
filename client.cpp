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
    std::string line;
    while(sendCommand(MsgID::Type::HELP, ""),
          printf("> "),
          std::getline(std::cin, line),
          line != "quit")
    {
      if( isdigit(line.c_str()[0]) )
      {
        switch(atoi(line.c_str()))
        {
          case 1:
            //ls
            fflush(stdin);
            line = '\0';
            printf("Please enter the directory you wish to list. \n");
            while( printf("> "), std::getline(std::cin, line), line.empty() );
            sendCommand(MsgID::Type::LS, line);
            break;
          case 2:
            //get
            fflush(stdin);
            line = '\0';
            printf("Please enter the name of the file you wish to GET. \n");
            while( printf("> "), std::getline(std::cin, line), line.empty() );
            sendCommand(MsgID::Type::GET, line);
            break;
          case 3:
            //put
            fflush(stdin);
            line = '\0';
            printf("Please enter the name of the file you wish to PUT. \n");
            while( printf("> "), std::getline(std::cin, line), line.empty() );
            sendCommand(MsgID::Type::PUT, line);
            break;
          case 4:
            //help
            sendCommand(MsgID::Type::HELP, "");
            break;
          case 0:
            line = "quit";
            break;
        }
      }
    }
  }
  close(m_connectionSocket);
}

int Client::createSocket()
{
  if((m_connectionSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    fprintf(stderr, "ERROR creating socket.\n");
  return m_connectionSocket;
}

int Client::Send(void *msg, int size)
{
  m_send = send(m_connectionSocket, networkize(msg), size, 0);
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
  m_receive = recv(m_connectionSocket, &m_currentMsg, sizeof(Header), 0);
  int msgSize = hostize(m_currentMsg).size - sizeof(Header);
  m_buffer = (char *)malloc(msgSize);
  zeroBuffer(m_buffer, msgSize);
  m_receive = recv(m_connectionSocket, m_buffer, msgSize, 0);
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
      return handleHelpCmd();
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
      zeroBuffer(ls.dir, 256);
      strcpy(ls.dir, command.c_str());
      ls.size = sizeof(LsCmd);
      return Send(&ls, ls.size);
    case MsgID::Type::GET:
      struct GetCmd get;
      get.msgId = type;
      strcpy(get.fileName, command.c_str());
      get.size = sizeof(GetCmd);
      return Send(&get, get.size);
    case MsgID::Type::PUT:
      struct PutCmd put;
      put.msgId = type;
      strcpy(put.fileName, command.c_str());
      put.size = sizeof(PutCmd);
      return Send(&put, put.size);
    case MsgID::Type::HELP:
      struct HelpCmd help;
      help.msgId = type;
      help.size = sizeof(HelpCmd);
      return Send(&help, help.size);
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
  free(m_buffer);
  return 0;
}

int Client::handlePutCmd()
{
  return 0;
}

int Client::handleHelpCmd()
{
  printf("%s\n", m_buffer);
  free(m_buffer);
  return 0;
}

int Client::getFileSize(char *fileName)
{
  FILE *file = fopen(fileName, "r");
  fseek(file, 0, SEEK_END);
  return ftell(file);
}

void Client::zeroBuffer(char *buffer, int size)
{
  bzero(buffer, size);
}

void *Client::networkize(void *msg)
{
  Header *tmpH = (Header *)msg;
  tmpH->size = htonl(tmpH->size);
  return msg;
}

Header Client::hostize(Header msg)
{
  msg.size = ntohl(msg.size);
  return msg;
}
