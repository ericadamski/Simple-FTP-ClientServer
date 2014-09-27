CC=g++
CFLAGS=-c -Wall -std=c++11 -g
CLANG=-std=c++11
HEADERS=messages.h

FTP: client.o server.o main.cpp
	    $(CC) $(CLANG) server.o client.o main.cpp -o FTP

server.o: messages.h server.h server.cpp
	          $(CC) $(CFLAGS) $(HEADER) server.cpp

client.o: messages.h client.h client.cpp
	          $(CC) $(CFLAGS) $(HEADER) client.cpp

clean:
	      rm -rf *.o FTP && clear
