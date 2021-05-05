CC = gcc 
CFLAGS = -g -Wall
LDFLAGS = -g 

default: filler_client filler_server

filler_client: filler_client.o filler_functions.o

filler_client.o: filler_client.c filler_functions.h

filler_server: filler_server.o filler_functions.o bot_functions.o queue.o

filler_server.o: filler_server.c filler_functions.h

filler_functions.o: filler_functions.c filler_functions.h

bot_functions.o: bot_functions.c bot_functions.h

queue.o: queue.c queue.h

.PHONY: clean
clean:
	rm -f filler_client filler_server *.a *.o

.PHONY: all
all: clean default
	