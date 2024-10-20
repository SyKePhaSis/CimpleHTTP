CC=gcc
HDIRS=Headers 
CFLAGS=-I $(HDIRS) -lws2_32 -Wall -Wextra -pedantic -fasynchronous-unwind-tables -fexceptions -fstack-clash-protection -O2 -Werror=format-security -ggdb
DEPS=Core/httpParser.h Utils/logger.h Core/routes.h Core/dataHandling.h Utils/fileHandling.h Utils/httpCreator.h app.h server.h
OBJ=Core/httpParser.o Utils/logger.o Core/routes.o Core/dataHandling.o Utils/fileHandling.o Utils/httpCreator.o app.o server.o
OUTPUT=server

$(BDIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)  

$(OUTPUT): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean: 
	rm -f $(OBJ) $(OUTPUT)