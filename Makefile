CC=g++
CFLAGS=-c -Wall -std=c++11 -g
CLANG=-std=c++11

FTP: client.o server.o main.cpp
	    $(CC) $(CLANG) server.o client.o main.cpp -o FTP

server.o: server.h server.cpp
	          $(CC) $(CFLAGS) server.cpp

client.o: client.h client.cpp
	          $(CC) $(CFLAGS) client.cpp

clean:
	      rm -rf *.o FTP && clear
