CC=gcc
HDIRS=Headers 
CFLAGS=-I $(HDIRS) -lws2_32 -Wall -Wextra -pedantic -fasynchronous-unwind-tables -fexceptions -fstack-clash-protection -O2 -Werror=format-security -g
COREDEPS= httpParser.h RoutingTable.h requestHandler.h routes.h dataHandling.h assetRouting.h
UTILDEPS= logger.h fileHandling.h httpCreator.h memmory.h
DEPS=$(addprefix Utils/,$(UTILDEPS)) $(addprefix Core/,$(COREDEPS))  app.h server.h
OBJ=$(addsuffix .o,$(basename $(DEPS)))
OBJDIR=test
OUTPUT=server

all: $(OUTPUT)
	@echo ====================================
	@echo  Successfully Completed compilation
	@echo ====================================

$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(OUTPUT): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: cleano clean

cleano: 
	rm -f $(OBJ)

clean: 
	rm -f $(OBJ) $(OUTPUT)
