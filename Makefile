CC=gcc
TARGET=AES_debug
INPUT_SRCS=AES_debug.c
INPUT_OBJS=$(INPUT_SRCS:.c=.o)

CFLAGS=-I /usr/include/openssl/ -ludev -o $(TARGET)
DEBUG=-g
LIBS=-lcrypto

all: $(INPUT_OBJS)
	$(CC) $(CFLAGS) $(LIBS) $^

debug: $(INPUT_OBJS)
	$(CC) $(DEBUG) $(CFLAGS) $(LIBS) $^

clean:
	rm -f $(TARGET) $(INPUT_OBJS)

