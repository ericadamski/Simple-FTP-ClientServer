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
  HelpCmd msg;
  msg.msgId = MsgID::Type::HELP;
  std::string help = "Options: \n\t[1] ls [dir] \n\t[2] get [filename] \n\t[3] put [filename] \n\t[4] help \n\t[0] Quit\n";
  strcpy(msg.helpMsg, help.c_str());
  msg.size = sizeof(HelpCmd);
  Send(&msg, msg.size, Server::MsgType::MSG);
}

int Server::Send(void *msg, int size, Server::MsgType flags)
{
  if( flags == Server::MsgType::MSG )
    m_send = send(m_acceptSocket, networkize(msg), size, 0);
  else
    m_send = send(m_acceptSocket, msg, size, 0);
  if( m_send < 0 )
  {
    fprintf(stderr, "Message send failure.");
    return -1;
  }
  return m_send;
}

int Server::Receive(int size)
{
  m_receive = recv(m_acceptSocket, &m_currentMsg, size, 0);
  int msgSize = hostize(m_currentMsg).size - sizeof(Header);
  m_buffer = (char *)malloc(msgSize);
  m_receive = recv(m_acceptSocket, m_buffer, msgSize, 0);
  if( m_receive < 0 )
  {
    fprintf(stderr, "ERROR reading message. \n");
    return -1;
  }
  return m_receive;
}

void Server::zeroBuffer(char *buffer, int size)
{ bzero(buffer, size); }

void Server::Listen()
{
  if(bind((m_connectionSocket = createSocket()),
      (struct sockaddr *) &m_address,
       sizeof(m_address)) < 0)
    error("ERROR on binding");
  else
  {
    listen(m_connectionSocket, 5);
    printf("Server is now listening on port %d.\n", m_port);
    m_clientLength = sizeof(m_clientAddress);
    m_acceptSocket = accept(m_connectionSocket,
                            (struct sockaddr *) &m_clientAddress, &m_clientLength);
    if(m_acceptSocket < 0)
    {
      fprintf(stderr, "ERROR on accept.\n");
    }
    while(sendResponse() >= 0);
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
  Receive(sizeof(Header));
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
  std::string cmd = GetStdoutFromCommand(LS_COMMAND.append(" ").append(m_buffer));
  m_buffer = (char *)malloc(strlen(cmd.c_str()));
  strcpy(m_buffer, cmd.c_str());
  struct Header msg;
  msg.msgId = MsgID::Type::LS;
  msg.size = strlen(m_buffer) + sizeof(Header);
  Send(&msg, msg.size, Server::MsgType::MSG);
  return Send(m_buffer, strlen(m_buffer), Server::MsgType::STRING);
}

int Server::handleGetCmd()
{
  return 0;
}

int Server::handlePutCmd()
{
  return 0;
}

std::string Server::GetStdoutFromCommand(std::string cmd)
{
  std::string data;
  FILE * stream;
  const int max_buffer = 256;
  char buffer[max_buffer];
  cmd.append(" 2>&1");

  stream = popen(cmd.c_str(), "r");
  if (stream)
  {
    while (!feof(stream))
      if (fgets(buffer, max_buffer, stream) != NULL) data.append(buffer);
        pclose(stream);
  }
  return data;
}

void *Server::networkize(void *msg)
{
  Header *tmpH = (Header *)msg;
  tmpH->size = htonl(tmpH->size);
  return msg;
}

Header Server::hostize(Header msg)
{
  msg.size = ntohl(msg.size);
  return msg;
}
