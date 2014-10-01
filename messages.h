#ifndef MESSAGES
#define MESSAGES  

#define MAX_BYTES 1024

#include <algorithm>

struct MsgID
{
  enum Type
  {
    LS,
    GET,
    PUT,
    HELP,
    QUIT
  };

  static const char* toString(Type msgId)
  {
    static char const * const lookup[] = {
      "ls",
      "Get",
      "Put",
      "Help",
      "Quit"
    };

    return lookup[msgId];
  }

  static int getMsgID(std::string type)
  {
    std::transform(type.begin(), type.end(), type.begin(), ::tolower);

    if(type == "ls")
      return 0;
    if(type == "get")
      return 1;
    if(type == "put")
      return 2;
    if(type == "help")
      return 3;
    if(type == "quit")
      return 4;
    return -1;
  }
};

struct Header
{
  MsgID::Type msgId;
  int size;
};

struct LsCmd : Header
{
  char dir[MAX_BYTES];
};

struct GetCmd : Header
{
  char fileName[MAX_BYTES];
  char file[MAX_BYTES];
};

struct PutCmd : Header
{
  char fileName[MAX_BYTES];
  char file[MAX_BYTES];
};

struct HelpCmd : Header
{
  char helpMsg[MAX_BYTES];
};

#endif
