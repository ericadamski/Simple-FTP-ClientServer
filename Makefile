CC=g++
CFLAGS=-c -Wall -std=c++11 -g
CLANG=-std=c++11
HEADERS=messages.h

FTP: client.o server.o file_utils.o main.cpp
	    $(CC) $(CLANG) -g server.o file_utils.o client.o main.cpp -o FTP

server.o: messages.h server.h server.cpp
	          $(CC) $(CFLAGS) $(HEADER) server.cpp

client.o: messages.h client.h client.cpp
	          $(CC) $(CFLAGS) $(HEADER) client.cpp

file_utils.o: file_utils.h file_utils.cpp
	             $(CC) $(CFLAGS) file_utils.cpp

clean:
	      rm -rf *.o FTP && clear
