CC=g++
TARGET=main
INPUT_SRCS=main.cpp emotiv.cpp
INPUT_OBJS=$(INPUT_SRCS:.cpp=.o)

CFLAGS=-I /usr/include/openssl/-o $(TARGET)
DEBUG=-g
LIBS=-lcrypto -ludev

all: $(INPUT_OBJS)
	$(CC) $(CFLAGS) $(LIBS) $^

debug: $(INPUT_OBJS)
	$(CC) $(DEBUG) $(CFLAGS) $(LIBS) $^

clean:
	rm -f $(TARGET) $(INPUT_OBJS)

