CC=g++
CFLAGS=-c -Wall

FTP: server.o main.cpp
	    $(CC) server.o main.cpp -o FTP

server.o: server.h server.cpp
	          $(CC) $(CFLAGS) server.cpp

client.o: client.h client.cpp
	          $(CC) $(CFLAGS) client.cpp

clean:
	      rm -rf *.o FTP
