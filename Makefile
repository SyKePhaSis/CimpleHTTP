CC=gcc
HDIRS=src/Headers 
CFLAGS=-I $(HDIRS) -lws2_32 -Wall -Wextra -pedantic -fasynchronous-unwind-tables -fexceptions -fstack-clash-protection -Werror=format-security
DEPS=$(patsubst %.c,%.h,$(wildcard src/Core/*.c)) 
DEPS+=$(patsubst %.c,%.h,$(wildcard src/Utils/*.c))
DEPS+=$(patsubst %.c,%.h,$(wildcard src/*.c))
DEPS+=$(patsubst %.c,%.h,$(wildcard Handlers/*.c))
OBJ=$(addprefix $(OBJDIR)/, $(addsuffix .o,$(basename $(DEPS))))
OBJDIR=build/objects
OUTPUT=app

HDIRS_CLI=cli/Headers/
CLI_CFLAGS=-Wall -I $(HDIRS_CLI) -Wextra -pedantic -fasynchronous-unwind-tables -fexceptions -fstack-clash-protection -Werror=format-security
DEPS_CLI=$(patsubst %.c,%.h, $(wildcard cli/*.c))
OBJ_CLI=$(addprefix $(OBJDIR)/, $(addsuffix .o,$(basename $(DEPS_CLI))))
OUTPUT_CLI=CimpleHTTP.exe

.PHONY: cleano clean debug cli test

all: header $(OUTPUT) footer

debug: CFLAGS += -DDEBUG -g
debug: header $(OUTPUT)
	@echo ========================================
	@echo Successfully Completed Debug Compilation
	@echo ========================================

cli: HDIRS = $(HDIRS_CLI)	
cli: header $(OUTPUT_CLI) footer

$(OBJDIR)/%.o: %.c
	@echo [*] Creating $@ Object File
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -c -o $@ $<

$(OUTPUT): $(OBJ)
	@echo [*] Creating $@ Execution File
	@$(CC) -o $@ $^ $(CFLAGS)

test:
	@echo $(DEPS)
	@echo $(OBJ)
	@echo $(DEPS_CLI)
	@echo $(OBJ_CLI)

$(OUTPUT_CLI): $(OBJ_CLI)
	@echo [*] Creating $@ Execution File
	@$(CC) -o $@ $^ $(CLI_CFLAGS)

cleano: 
	@rm -f $(OBJ)
	@echo [*] Cleaned Object Files

cleano-cli:
	@rm -f $(OBJ_CLI)
	@echo [*] Cleaned Object Files

clean-cli:
	@rm -f $(OBJ_CLI) $(OUTPUT_CLI)
	@echo [*] Cleaned All Files

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

