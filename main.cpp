#include "server.h"
#include <string>
#include <iostream>
#include <stdlib.h>

int main(int argc, char* argv[])
{
  const char* HELP = "Usage: FTP [ip_address] [port]\n";
  const char* MENU = "Options : \n\t [1] Begin Client \n\t [2] Begin Server \n\t [0] Quit \n";

  Server *m_server;

  if(argc < 2)
  {
    printf("%s", HELP);
    return -1;
  }

  while(std::cin)
  {
    std::string line;
    printf("%s", MENU);
    std::getline(std::cin, line);

    switch(std::stoi(line))
    {
      case 0:
        return 0;
      case 1:
        // run client code connecting to IP and Port provided
        break;
      case 2:
        // run server code.
        m_server = new Server();
        m_server->Listen();
        delete(m_server);
        break;
    }
  } 
}
