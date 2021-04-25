CC = gcc 
CFLAGS = -g -Wall

LDFLAGS = -g

default: filler_client filler_server

filler_client: filler_client.o

filler_client.o: filler_client.c

filler_server: filler_server.o

filler_server.o: filler_server.c

.PHONY: clean
clean:
	rm -f filler_client filler_server *.o

.PHONY: all
make:
	clean default
