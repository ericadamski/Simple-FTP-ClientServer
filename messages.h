#ifndef MESSAGES
#define MESSAGES

struct MsgID
{
  enum Type
  {
    LS,
    GET,
    PUT,
    HELP
  };

  static const char* toString(Type msgId)
  {
    static char const * const lookup[] = {
      "ls",
      "Get",
      "Put",
      "Help"
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
  char dir[256];
};

struct GetCmd : Header
{
  char fileName[256];
};

struct PutCmd : Header
{
  char fileName[256];
  int  fileSize;
};

struct HelpCmd : Header
{
  char helpMsg[256];
};

#endif
