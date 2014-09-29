#ifndef FILEUTILS
#define FILEUTILS

#include <stdio.h>
#include <iostream>

class FileUtils
{
public:
  static std::string getFile(char*);
  static void putFile(FILE*);
  static int getFileSize(char*);
};

#endif //FILEUTILS
