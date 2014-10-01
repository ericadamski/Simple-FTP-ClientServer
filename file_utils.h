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
  static std::string listDirectory(const char*);
};

#endif //FILEUTILS
