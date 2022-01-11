#make c forditáshoz v 1.1
CFLAGS= -O0 -g3
OBJS=$(patsubst %.c,%.o,$(wildcard *.c))
BIN=main
CC=gcc
COFLAGS=-l
CIFLAGS=-u
CO=co
CI=ci

SERVER_ADDR=192.168.1.52
SERVER_PORT=7654

TGZ=tar
TGZFL=-cuzf
TGZ_FILE=cuc.tgz
PEN=/media/pendrive
NAME=`pwd`

.PHONY:bin

client:client.c
	$(CC) -o client client.c
	./client $(SERVER_ADDR) $(SERVER_PORT)

server:server.c
	$(CC) -o server server.c
	./server $(SERVER_PORT)

.PHONY: clean

clean:
	$(RM) $(OBJS)
	$(RM) $(BIN)
