#ifndef FILEUTILS
#define FILEUTILS

#include <stdio.h>
#include <iostream>

/* Class FileUtils
 * 	Used for all file operations within the program
*/
class FileUtils
{
public:
  //Get a file using the file name
  static std::string getFile(const char*);

  //Write a file with the given file name and data
  static void putFile(const char*, const char*);

  //Get the size of a given file
  static int getFileSize(char*);

  //List the current directory, equivilent to '/bin/ls' command
  static std::string listDirectory(const char*);
};

#endif //FILEUTILS
