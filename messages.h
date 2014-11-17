#ifndef MESSAGES
#define MESSAGES  

//For all defined arrays
#define MAX_BYTES 1024

//For easy translation from client <--> server
// using an enum allows for switch statements, which in turn provides cleaner
// code. 
struct MsgID
{
  //All the different types of messages.
  enum Type
  {
    LS,
    GET,
    PUT,
    HELP,
    QUIT
  };

  //Given a Type this method gives back the string representation of that type
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

  //Given a string representation of the type, this method returns the integer
  // value of that type.
  static int getMsgID(std::string type)
  {
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

//Basic network header
// @member msgId : the ID of the operation to preform, 
//                 must be of type MsgID::Type
// @member size  : the total size of the transaction including the size of 
//                 the header itself.
struct Header
{
  MsgID::Type msgId;
  int size;
};

//List command network header
// @member dir[] : the directory to list
struct LsCmd : Header
{
  char dir[MAX_BYTES];
};

//Get remote-file network header
// @member fileName[] : the name of the file for which to get
struct GetCmd : Header
{
  char fileName[MAX_BYTES];
};

//Put local-file command network header
// @member fileName[] : the name of the local file to send to the server.
struct PutCmd : Header
{
  char fileName[MAX_BYTES];
};

//Help command network header
// @member helpMsg[] : the list of commands available on the server.
struct HelpCmd : Header
{
  char helpMsg[MAX_BYTES];
};

#endif
