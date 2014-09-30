#ifndef FILEUTILS
#define FILEUTILS

#include <stdio.h>
#include <iostream>

class FileUtils
{
public:
  static std::string getFile(const char*);
  static void putFile(const char*, const char*);
  static int getFileSize(char*);
};

#endif //FILEUTILS
