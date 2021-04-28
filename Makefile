CC = gcc 
CFLAGS = -g -Wall

LDFLAGS = -g

default: filler_client filler_server

filler_client: filler_client.o filler_functions.o

filler_client.o: filler_client.c filler_functions.h

filler_server: filler_server.o filler_functions.o

filler_server.o: filler_server.c filler_functions.h

filler_functions.o: filler_functions.c filler_functions.h

.PHONY: clean
clean:
	rm -f filler_client filler_server *.o

.PHONY: all
all: clean default
