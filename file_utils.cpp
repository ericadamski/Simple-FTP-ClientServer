#include "file_utils.h"
#include <iostream>
#include <string>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <dirent.h>

/* Algorithm 
 *  if you can open the file using a stream
 *    read the file one character at a time
 *  close the file stream
 *  return the data as a string.
 *
 *  @param name : name of the file to read from
*/
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

/* Algorithm
 *  if the filename is from a different directory other then '.'
 *    remove all of the preceding directories until only the file remains.
 *  open a file with that given name as a output stream
 *  write the given data to the file
 *  close the file
 *
 *  @param filename : name of the file to write to
 *  @param data     : the data to write to the file
*/
void FileUtils::putFile(const char *filename, const char *data)
{
  std::string sfile (filename);
  unsigned int local = sfile.find_last_of("/\\");
  sfile = sfile.substr(local + 1);
  std::ofstream file (sfile);
  file.write(data, strlen(data));
  file.close();
}

//Count the number of characters in the file
int FileUtils::getFileSize(char *name)
{
  int size = -1;
  FILE *file = fopen(name, "r");
  fseek(file, 0, SEEK_END);
  size = ftell(file);
  fclose(file);
  return size;
}

//List the directories using the built in C struct dirent
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
