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
    while(printf("> "), std::getline(std::cin, line), line != "quit")
    {
      if(!line.empty() || line != "quit")
      {
        Send(line);
        fflush(stdin);
        line = '\0';
        Receive();
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

int Client::Send(std::string msg)
{
  zeroBuffer();
  if( sizeof(msg) > 255 )
  {
    fprintf(stderr, "ERROR Message is to large.");
    return -1;
  }
  strcpy(buffer, msg.c_str());
  printf("SEND : %s", buffer);
  m_send = send(m_connectionSocket, buffer, sizeof(buffer), 0);
  if( m_send < 0 )
  {
    fprintf(stderr, "Message send failure.");
    return -1;
  }
  printf("message sent\n");
  return 0;
}

int Client::Receive()
{
  zeroBuffer();
  m_receive = recv(m_connectionSocket, buffer, 256, 0);
  if( m_receive < 0 )
  {
    fprintf(stderr, "ERROR receiving message.");
    return -1;
  }
  fflush(stdout);
  printf("RECEIVE : %s\n", buffer);
  return 0;
}

void Client::zeroBuffer()
{
  bzero(buffer, 256);
}
