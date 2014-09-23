CC=g++
CFLAGS=-c -Wall
CLANG=-std=c++11

FTP: server.o main.cpp
	    $(CC) $(CLANG) server.o main.cpp -o FTP

server.o: server.h server.cpp
	          $(CC) $(CFLAGS) server.cpp

client.o: client.h client.cpp
	          $(CC) $(CFLAGS) client.cpp

clean:
	      rm -rf *.o FTP && clear
