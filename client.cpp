#include "client.h"

/*
 *  Constructor
 *  
 *  @param addr : IPv4 address to connect to
 *  @param port : the port on which to connect
 */
Client::Client(std::string addr, std::string port)
{
  if( addr.empty() || port.empty() )
  {
    fprintf(stderr, "Address or Port is empty.\n");
    return;
  }
  m_portNumber = atoi(port.c_str());
  m_server = gethostbyname(addr.c_str());

  if(m_server == NULL)
  {
    fprintf(stderr, "ERROR no such host.\n");
    return;
  }

  bzero((char *) &m_serverAddress, sizeof(m_serverAddress));
  m_serverAddress.sin_family = AF_INET;

  bcopy((char *)m_server->h_addr,
    (char *) &m_serverAddress.sin_addr.s_addr,
    m_server->h_length);
  m_serverAddress.sin_port = htons(m_portNumber);
}

/* Destructor */
Client::~Client(){}

/* Main Loop 
 *
 *  Open a socket and try to connect with the server.
 *  If there is no error request the help from the server and wait for input
 *
 *  when the main loop quits, a quit command is sent to the server and the 
 *  pipe is closed
 */
void Client::Connect()
{
  if(connect(createSocket(),
    (struct sockaddr *) &m_serverAddress,
    sizeof(m_serverAddress)) < 0)
    fprintf(stderr, "ERROR connecting to server.\n");
  else
  {
    printf("Please enter a command.\n");
    std::string line;
    while(sendCommand(MsgID::Type::HELP, ""),
          printf("> "),
          std::getline(std::cin, line),
          line != "quit")
    {
      int msgId = MsgID::getMsgID(line);
      if( msgId != -1 )
      {
        switch(msgId)
        {
          case 0:
            //ls
            fflush(stdin);
            line = '\0';
            printf("Please enter the directory you wish to list. \n");
            while( printf("> "), std::getline(std::cin, line), line.empty() );
            sendCommand(MsgID::Type::LS, line);
            break;
          case 1:
            //get
            fflush(stdin);
            line = '\0';
            printf("Please enter the name of the file you wish to GET. \n");
            while( printf("> "), std::getline(std::cin, line), line.empty() );
            m_fileName = line;
            sendCommand(MsgID::Type::GET, line);
            break;
          case 2:
            //put
            fflush(stdin);
            line = '\0';
            printf("Please enter the name of the file you wish to PUT. \n");
            while( printf("> "), std::getline(std::cin, line), line.empty() );
            m_fileName = line;
            sendCommand(MsgID::Type::PUT, line);
            break;
          case 3:
            //help
            fflush(stdin);
            line = '\0';
            sendCommand(MsgID::Type::HELP, "");
            break;
          case 4:
            sendCommand(MsgID::Type::QUIT, "");
            return;
        }
      }
    }
  }
  sendCommand(MsgID::Type::QUIT, "");
  close(m_connectionSocket);
}

/* Create a client socket */
int Client::createSocket()
{
  if((m_connectionSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    fprintf(stderr, "ERROR creating socket.\n");
  return m_connectionSocket;
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
int Client::Send(void *msg, int size, Client::MsgType flags)
{
  if(flags == Client::MsgType::MSG)
    m_send = send(m_connectionSocket, networkize(msg), size, 0);
  else
    m_send = send(m_connectionSocket, msg, size, 0);
  sleep(1);
  if( m_send < 0 )
  {
    fprintf(stderr, "Message send failure.");
    return -1;
  }
  printf(" message sent\n");
  return m_send;
}

// The start of the receive process
int Client::Receive()
{
  return ReceiveHeader();
}

// Receiving the Header which contains the operation to preform 
//  and the size of the message
// will return and error if < 0 bytes are received.
int Client::ReceiveHeader()
{
  m_receive = recv(m_connectionSocket, &m_currentMsg, sizeof(Header), 0);
  printf("Received %d bytes.", m_receive); //should always be 8
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
int Client::ReceiveData(int size)
{
  int bytesToReceive = size;

  printf("Need to receive %d bytes.", size);
  m_buffer = (char *)malloc(MAX_BYTES);
  m_data = "";
  while( bytesToReceive > 0 )
  {
    zeroBuffer(m_buffer, MAX_BYTES);
    m_receive = recv(m_connectionSocket, m_buffer, MAX_BYTES, 0);
    printf("Received %d bytes.", m_receive);
    if( m_receive < 0 )
    {
      fprintf(stderr, "ERROR receiving message.");
      return -1;
    }
    m_data.append(m_buffer);
    bytesToReceive -= m_receive;
  }

  return size;
}

/* Send the given command, and handle the response, if there is one. */
int Client::sendCommand(MsgID::Type type, std::string command)
{
  sendHeader(type, command);
  switch(type)
  {
    case MsgID::Type::LS:
      Receive();
      return handleLsCmd();
    case MsgID::Type::GET:
      Receive();
      return handleGetCmd();
    case MsgID::Type::PUT:
      return handlePutCmd();
    case MsgID::Type::HELP:
      Receive();
      return handleHelpCmd();
    case MsgID::Type::QUIT:
      return 0;
    default:
      return -1;
  }
}

/* Send the requests to the server */
int Client::sendHeader(MsgID::Type type, std::string command)
{
  switch(type)
  {
    case MsgID::Type::LS:
      struct LsCmd ls;
      ls.msgId = type;
      zeroBuffer(ls.dir, MAX_BYTES);
      strcpy(ls.dir, command.c_str());
      ls.size = sizeof(LsCmd);
      return Send(&ls, ls.size, MsgType::MSG);
    case MsgID::Type::GET:
      struct GetCmd get;
      get.msgId = type;
      zeroBuffer(get.fileName, MAX_BYTES);
      strcpy(get.fileName, command.c_str());
      get.size = sizeof(GetCmd);
      return Send(&get, get.size, MsgType::MSG);
    case MsgID::Type::PUT:
      struct PutCmd put;
      put.msgId = type;
      zeroBuffer(put.fileName, MAX_BYTES);
      strcpy(put.fileName, command.c_str());
      put.size = sizeof(PutCmd);
      return Send(&put, put.size, MsgType::MSG);
    case MsgID::Type::HELP:
      struct HelpCmd help;
      help.msgId = type;
      help.size = sizeof(HelpCmd);
      return Send(&help, help.size, MsgType::MSG);
    case MsgID::Type::QUIT:
      struct Header quit;
      quit.msgId = type;
      quit.size = sizeof(Header);
      return Send(&quit, quit.size, MsgType::MSG);
    default:
      return -1;
  }
}

// Write the given data to the file requested
int Client::handleGetCmd()
{
  FileUtils::putFile(m_fileName.c_str(), m_data.c_str());
  return 0;
}

// Print the output of the LS command to stdout
int Client::handleLsCmd()
{
  printBuffer();
  return 0;
}

/*
 *  Algorithm
 *    Read in the file which needs to be sent to the server
 *    Send a header informing the server of the number of bytes to be prepared
 *    to receive, then in chunks of 1024 bytes, send the file
 *
 *  @return : the number of bytes sent in the last transaction
 */
int Client::handlePutCmd()
{
  struct Header msg;
  msg.msgId = MsgID::Type::PUT;
  std::string file = FileUtils::getFile(m_fileName.c_str());
  std::string substring = "";
  int total = file.length();
  msg.size = sizeof(Header) + total;
  Send(&msg, sizeof(Header), MsgType::MSG);
  int end = 0;
  while(!(substring = file.substr(end, MAX_BYTES)).empty() &&
        !(end == total))
  {
    if( end + MAX_BYTES < total )
      end += MAX_BYTES;
    else
      end = total;
    Send((void *)substring.c_str(), substring.length(), MsgType::STRING);
    substring = "";
  }
  return m_send;
}

// Print the output of the HELP command to stdout
int Client::handleHelpCmd()
{
  printBuffer();
  return 0;
}

// Print and free the buffer
void Client::printBuffer()
{
  printf("Printing and freeing buffer\n");
  printf("%s\n", m_buffer);
  zeroBuffer(m_buffer, strlen(m_buffer));
  free(m_buffer);
}

// Sets all entries of a buffer to '\0'
void Client::zeroBuffer(char *buffer, int size)
{
  for(int i = 0; i < size; i++)
    buffer[i] = '\0';
}

// Transforms longs to network byte order
void *Client::networkize(void *msg)
{
  Header *tmpH = (Header *)msg;
  tmpH->size = htonl(tmpH->size);
  return msg;
}

// Transforms longs to host byte order
Header Client::hostize(Header msg)
{
  msg.size = ntohl(msg.size);
  return msg;
}
