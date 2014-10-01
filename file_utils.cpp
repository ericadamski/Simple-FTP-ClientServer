#include "file_utils.h"
#include <iostream>
#include <string>
#include <stdio.h>
#include <fstream>
#include <dirent.h>

std::string FileUtils::getFile(const char *name)
{
  std::string data = "";
  std::ifstream file (name);
  if( file.is_open() )
  {
    char c;
    while(file.get(c))
      data.append(1, c);
    file.close();
  }

  return data;
}

void FileUtils::putFile(const char *filename, const char *data)
{
  std::string sfile (filename);
  unsigned int local = sfile.find_last_of("/\\");
  sfile = sfile.substr(local + 1);
  std::ofstream file (sfile);
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

std::string FileUtils::listDirectory(const char *dir)
{
  DIR *dp;
  struct dirent *ep;
  std::string data = "";

  dp = opendir (dir);
  if (dp != NULL)
    {
      while ((ep = readdir (dp)))
        data.append(ep->d_name).append("\n");
      (void) closedir (dp);
    }
  else
    data = "Couldn't open the directory";

  return data;
}