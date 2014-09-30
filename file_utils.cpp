#include "file_utils.h"
#include <iostream>
#include <stdio.h>
#include <fstream>

std::string FileUtils::getFile(char *name)
{
  std::string data = "";
  std::ifstream file (name);
  if( file.is_open() )
  {
    char c;
    while(file.get(c) != NULL)
      data.append(1, c);
    file.close();
  }

  return data;
}

void FileUtils::putFile(char *filename, const char *data)
{
  std::ofstream file (filename);
  file << data;
  file.close();
}

int FileUtils::getFileSize(char *name)
{
  int size = -1;
  FILE *file = fopen(name, "r");
  fseek(file, 0, SEEK_END);
  size = ftell(file);
  fclose(file);
  return size;
}
