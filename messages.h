#ifndef MESSAGES
#define MESSAGES

#define MAX_BYTES 1024

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
