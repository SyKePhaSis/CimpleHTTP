CC = gcc
CFLAGS = -lws2_32
DEPS = Utils/httpParser.h Utils/logger.h
OBJ = Utils/httpParser.o Utils/logger.o server.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

server: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)