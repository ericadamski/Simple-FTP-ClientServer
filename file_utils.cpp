#include "file_utils.h"
#include <iostream>
#include <stdio.h>
#include <fstream>

std::string FileUtils::getFile(char *name)
{
  std::string line = "",
              data = "";
  std::ifstream file (name);
  if( file.is_open() )
  {
    while(std::getline(file, line))
      data.append(line);
    file.close();
  }

  return data;
}

void FileUtils::putFile(FILE *file)
{

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
