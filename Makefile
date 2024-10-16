CC = gcc
CFLAGS = -lws2_32 -Wall -Wextra -pedantic -fasynchronous-unwind-tables -fexceptions -fstack-clash-protection -O2 -Werror=format-security -ggdb
DEPS = Utils/logger.h Core/httpParser.h Core/routes.o Core/dataHandling.h Utils/fileHandling.h Utils/httpCreator.h 
OBJ =  Utils/logger.o Core/httpParser.o Core/routes.o Core/dataHandling.o Utils/fileHandling.o Utils/httpCreator.o server.o 
OUTPUT=server

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(OUTPUT): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean: 
	rm -f $(OBJ) $(OUTPUT)