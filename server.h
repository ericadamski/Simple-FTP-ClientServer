#ifndef SERVER
#define SERVER

#include <list>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include "messages.h"

class Server
{
  public:

    // Used to filter byte streams from short/long(s)
    enum MsgType
    {
      MSG,
      STRING,
      OTHER
    };

    /* Constructor */
    Server();
    /* Constructor
     *  @param port : port number to listen on
     */
    Server(std::string);
    /* Destructor */
    ~Server();

    void Close();
    void error(const char*);
    void Listen();
    
    //takes a void pointer and the enum MsgType so that a generic send method 
    //  can be used to send all types of data over the network.
    int Send(void*, int, MsgType);
    int Receive();

  private:
    // Used for testing
    const int PORT = 30000;

    // All data from over the network gets buffered into this string object
    std::string m_data;
    // if files are being transfered the file name is stored here.
    std::string m_fileName;

    // Used to buffer data coming from over the network, or reading in files.
    char *m_buffer;

    // The server and client socekts.
    struct sockaddr_in m_address;
    struct sockaddr_in m_clientAddress;

    socklen_t m_clientLength;

    // The current operation information ( MsgID::Type and total size )
    struct Header m_currentMsg;

    int ReceiveHeader();
    int ReceiveData(int);

    int m_connectionSocket;
    int m_acceptSocket;
    int m_send;
    int m_receive;
    int m_port;

    int createSocket();
    int sendResponse();
    int handleGetCmd();
    int handleLsCmd();
    int handlePutCmd();

    void zeroBuffer(char*, int);
    void printHelp();

    void *networkize(void*);
    
    Header hostize(Header);

    std::string GetStdoutFromCommand(std::string);
};

#endif
