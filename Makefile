CC=gcc
HDIRS=src/Headers 
CFLAGS=-I $(HDIRS) -lws2_32 -Wall -Wextra -pedantic -fasynchronous-unwind-tables -fexceptions -fstack-clash-protection -Werror=format-security
DEPS=$(patsubst %.c,%.h,$(wildcard src/Core/*.c)) 
DEPS+=$(patsubst %.c,%.h,$(wildcard src/Utils/*.c))
DEPS+=$(patsubst %.c,%.h,$(wildcard src/*.c)) 
OBJ=$(addprefix $(OBJDIR)/, $(addsuffix .o,$(basename $(DEPS))))
OBJDIR=build/objects
OUTPUT=server

.PHONY: cleano clean debug 

all: header $(OUTPUT) footer

debug: CFLAGS += -DDEBUG -g
debug: header $(OUTPUT)
	@echo ========================================
	@echo Successfully Completed Debug Compilation
	@echo ========================================

$(OBJDIR)/%.o: %.c
	@echo [*] Creating $@ Object File
	@$(CC) $(CFLAGS) -c -o $@ $<

$(OUTPUT): $(OBJ)
	@echo [*] Creating $@ Execution File
	@$(CC) -o $@ $^ $(CFLAGS)

cleano: 
	@rm -f $(OBJ)
	@echo [*] Cleaned Object Files

clean: 
	@rm -f $(OBJ) $(OUTPUT)
	@echo [*] Cleaned All Files

header: 
	@echo ====================
	@echo Starting Compilation
	@echo ====================

footer:
	@echo ==================================
	@echo Successfully Completed compilation
	@echo ==================================

