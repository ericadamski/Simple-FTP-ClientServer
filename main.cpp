#include "server.h"
#include "client.h"
#include <string>
#include <iostream>
#include <stdlib.h>

int main(int argc, char* argv[])
{
  const char* HELP = "Usage: FTP [ip_address] [port]\n";
  const char* MENU = "Options : \n\t [1] Begin Client \n\t [2] Begin Server \n\t [0] Quit \n";

  Server *m_server;
  Client *m_client;

  if(argc < 3)
  {
    printf("%s", HELP);
    return -1;
  }

  while(std::cin)
  {
    std::string line;
    printf("%s", MENU);
    std::getline(std::cin, line);

    printf("Please enter the number corresponding to the command to run.\n");

    if( !line.empty() && isdigit(line.c_str()[0]) )
    {
      switch(std::stoi(line))
      {
       case 0:
         return 0;
       case 1:
         // run client code connecting to IP and Port provided
         m_client = new Client(argv[1], argv[2]);
         m_client->Connect();
         delete(m_client);
         break;
       case 2:
         // run server code
         m_server = new Server(argv[2]);
         m_server->Listen();
         delete(m_server);
         break;
      }
    }
  } 
}
