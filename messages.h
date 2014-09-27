#ifndef MESSAGES
#define MESSAGES

struct MsgID
{
  enum Type
  {
    LS,
    GET,
    PUT
  };

  static const char* toString(Type msgId)
  {
    static char const * const lookup[] = {
      "ls",
      "Get",
      "Put"
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

struct LsCmdResponse
{
  int lsSize; //number of bytes of ls cmd output
};

struct GetCmdResponse
{
  int getSize; //number of bytes of file requested, -1 if error
};

struct PutCmdResponse
{
  int result; //0 if success 1 otherwise
};

#endif
