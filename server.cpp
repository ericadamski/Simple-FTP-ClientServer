#include "server.h"
#include "file_utils.h"

Server::Server()
{
  bzero((char *) &m_address, sizeof(m_address));
  m_address.sin_family = AF_INET;
  m_address.sin_addr.s_addr = INADDR_ANY;
  m_address.sin_port = htons(PORT);
  m_free = 0;
}

Server::Server(std::string port)
{
  if(port.empty())
    return;

  m_fileName = "";
  bzero((char *) &m_address, sizeof(m_address));
  m_address.sin_family = AF_INET;
  m_address.sin_addr.s_addr = INADDR_ANY;
  m_address.sin_port =
    htons((m_port = atoi(port.c_str())));
  m_free = 0;
}

Server::~Server()
{
  if( m_free == 1)
    free(m_buffer);
}

void Server::error(const char* message)
{
  perror(message);
}

// Create the Socket on which to listen.
int Server::createSocket()
{
  int socketId;
  if((socketId = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    error("Error creating socket.");
  return socketId;
}

// Close both the listen and accept socekts
void Server::Close()
{
  close(m_connectionSocket);
  close(m_acceptSocket);
}

// Send the help command ( when requested ) to the client
void Server::printHelp()
{
  HelpCmd msg;
  msg.msgId = MsgID::Type::HELP;
  std::string help = "Options: \n\t[1] ls \n\t[2] get \n\t[3] put \n\t[4] help \n\t[0] quit\n";
  strcpy(msg.helpMsg, help.c_str());
  msg.size = sizeof(HelpCmd);
  Send(&msg, msg.size, Server::MsgType::MSG);
}

/* Send the information stored in msg over the network
 *  check for errors in sending and return error otherwise return the 
 *  number of bytes sent.
 *
 *  @param msg   : the data to be send across the network
 *  @param size  : the size of the data to be sent
 *  @param flags : this determines if byte order conversions need to happen
 *
 *  return value : number of bytes sent, -1 otherwise
 */
int Server::Send(void *msg, int size, Server::MsgType flags)
{
  if( flags == Server::MsgType::MSG )
    m_send = send(m_acceptSocket, networkize(msg), size, 0);
  else
    m_send = send(m_acceptSocket, msg, size, 0);
  sleep(1);
  if( m_send < 0 )
  {
    fprintf(stderr, "Message send failure.");
    return -1;
  }
  return m_send;
}

// The start of the receive process
int Server::Receive()
{
  return ReceiveHeader();
}

// Receiving the Header which contains the operation to preform 
//  and the size of the message
// will return and error if < 0 bytes are received.
int Server::ReceiveHeader()
{
  m_receive = recv(m_acceptSocket, &m_currentMsg, sizeof(Header), 0);
  if( m_receive < 0 )
  {
    fprintf(stderr, "ERROR receiving message.");
    return -1;
  }
  return ReceiveData(hostize(m_currentMsg).size - sizeof(Header));
}

/*
 *  @param size : the total number of bytes to be expected 
 *
 *  Algorithm
 *    read a constant number of bytes into a buffer until there is an error
 *    or no more bytes to receive.
 *
 *  @return : the number of bytes received or -1
 */
int Server::ReceiveData(int size)
{
  int bytesToReceive = size;

  allocBuffer(MAX_BYTES);
  m_data = "";

  while( bytesToReceive > 0 )
  {
    zeroBuffer(m_buffer, MAX_BYTES);
    m_receive = recv(m_acceptSocket, m_buffer, MAX_BYTES, 0);
    if( m_receive < 0 )
    {
      fprintf(stderr, "ERROR reading message. \n");
      return -1;
    }
    m_data.append(m_buffer);
    bytesToReceive -= m_receive;
  }

  return size;
}

// Sets all entries of a buffer to '\0'
void Server::zeroBuffer(char *buffer, int size)
{ bzero(buffer, size); }

/*
 *  Bind to a socket, listen on that socket for connections
 *  if there is an error connecting close the socket and return
 */
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
                            (struct sockaddr *) &m_clientAddress,
                            &m_clientLength);
    if(m_acceptSocket < 0)
    {
      fprintf(stderr, "ERROR on accept.\n");
    }
    else
      while(sendResponse() >= 0);
  }
  Close();
}

// Handle all types of requests from the Client
int Server::sendResponse()
{
  Receive();
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
    case MsgID::Type::QUIT:
      return -2;
    default:
      return -1;
  }
}

/*
 * Send a message to the client with the results of an '/bin/ls'
 */
int Server::handleLsCmd()
{
  std::string cmd = GetStdoutFromCommand(std::string (m_buffer));
  struct LsCmd msg;
  msg.msgId = MsgID::Type::LS;
  msg.size = sizeof(LsCmd);
  strcpy(msg.dir, cmd.c_str());
  Send(&msg, msg.size, Server::MsgType::MSG);
  return m_send;
}

/*
 *  Algorithm
 *    Read in the file which the client wants to get
 *    Send a header informing the client of the number of bytes to be prepared
 *    to receive, then in chunks of 1024 bytes, send the file
 *
 *  @return : the number of bytes sent in the last transaction
 */
int Server::handleGetCmd()
{
  struct Header msg;
  msg.msgId = MsgID::Type::GET;
  std::string file = FileUtils::getFile(m_data.c_str());
  std::string substring = "";
  int total = file.length();
  msg.size = sizeof(Header) + total;
  Send(&msg, sizeof(Header), Server::MsgType::MSG);
  int end = 0;
  while(!(substring = file.substr(end, MAX_BYTES)).empty() &&
        !(end == total))
  { 
    if( end + MAX_BYTES < total )
      end += MAX_BYTES;
    else
      end = total;
    Send((char *)substring.c_str(),
         substring.length(), Server::MsgType::STRING);
    substring = "";
  } 
  return m_send;
}

// Write the given data to the file
int Server::handlePutCmd()
{
  if(!m_fileName.empty())
  {
    FileUtils::putFile(m_fileName.c_str(), m_data.c_str());
    m_fileName = "";
    return 0;
  }
  else
  {
    m_fileName = m_data;
    Receive();
    return handlePutCmd();
  }
}

// execute LS command
std::string Server::GetStdoutFromCommand(std::string cmd)
{
  return FileUtils::listDirectory(cmd.c_str());
}

// Transforms longs to network byte order
void *Server::networkize(void *msg)
{
  Header *tmpH = (Header *)msg;
  tmpH->size = htonl(tmpH->size);
  return msg;
}

// Transforms longs to host byte order
Header Server::hostize(Header msg)
{
  msg.size = ntohl(msg.size);
  return msg;
}

// Allocates memory into the buffer and creates
// a flag so it can be free'ed
void Server::allocBuffer(int size)
{
  if( m_free == 1 )
  {
    free(m_buffer);
    m_free = 0;
  }

  m_buffer = (char *)malloc(size);
  m_free = 1;
  zeroBuffer(m_buffer, size);
}
