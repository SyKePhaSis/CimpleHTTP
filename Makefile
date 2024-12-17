CC=gcc
HDIRS=src/Headers 
CFLAGS=-I $(HDIRS) -lws2_32 -Wall -Wextra -pedantic -fasynchronous-unwind-tables -fexceptions -fstack-clash-protection -Werror=format-security
DEPS=$(patsubst %.c,%.h,$(wildcard src/Core/*.c)) 
DEPS+=$(patsubst %.c,%.h,$(wildcard src/Utils/*.c))
DEPS+=$(patsubst %.c,%.h,$(wildcard src/DataTypes/*.c))
DEPS+=$(patsubst %.c,%.h,$(wildcard src/*.c))
# DEPS+=$(patsubst %.c,%.h,$(wildcard Handlers/*.c))
OBJ=$(addprefix $(OBJDIR)/, $(addsuffix .o,$(basename $(DEPS))))
OBJDIR=build/objects
OUTPUT_DLLS=$(addprefix $(OBJDIR)/, $(patsubst %.c,%.dll,$(wildcard Handlers/*.c)))
OUTPUT=app

HDIRS_CLI=cli/Headers/
CLI_CFLAGS=-Wall -I $(HDIRS_CLI) -Wextra -pedantic -fasynchronous-unwind-tables -fexceptions -fstack-clash-protection -Werror=format-security
DEPS_CLI=$(patsubst %.c,%.h, $(wildcard cli/*.c))
OBJ_CLI=$(addprefix $(OBJDIR)/, $(addsuffix .o,$(basename $(DEPS_CLI))))
OUTPUT_CLI=CimpleHTTP.exe

TIMER_START  := $(shell date "+%s")
TIMER_END     = $(shell date "+%s")
TIMER_SECONDS = $(shell expr $(TIMER_END) - $(TIMER_START))
TIMER_FORMAT  = $(shell date --utc --date="@$(TIMER_SECONDS)" "+%H:%M:%S")

.PHONY: handlers clean-handlers cleano clean debug cli test clean-cli cleano-cli

all: header $(OUTPUT) $(OUTPUT_DLLS) $(OUTPUT_CLI) footer

debug: CFLAGS += -DDEBUG -g -ggdb
debug: header $(OUTPUT) $(OUTPUT_DLLS)
	@echo ========================================
	@echo Successfully Completed Debug Compilation
	@echo ========================================

cli: HDIRS = $(HDIRS_CLI)	
cli: header $(OUTPUT_CLI) footer

$(OBJDIR)/%.o: %.c
	@echo -n [$(TIMER_FORMAT)] Creating $@ Object File
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -c -o $@ $<
	@echo -e " [OK]"

$(OUTPUT): $(OBJ)
	@echo -n [$(TIMER_FORMAT)] Creating $@ Execution File
	@$(CC) -o $@ $^ $(CFLAGS)
	@echo -e " [OK]"

test:
	@echo $(DEPS)
	@echo $(OBJ)
	@echo $(DEPS_CLI)
	@echo $(OBJ_CLI)

$(OUTPUT_CLI): $(OBJ_CLI)
	@echo -n [$(TIMER_FORMAT)] Creating $@ Execution File
	@$(CC) -o $@ $^ $(CLI_CFLAGS)
	@echo -e " [OK]"

$(OBJDIR)/Handlers/%.dll: ./Handlers/%.c $(OBJ)
	@echo -n [$(TIMER_FORMAT)] Creating $@ DLL File
	@$(CC) --shared -o $@ $^ $(CFLAGS)
	@echo -e " [OK]"

handlers: $(OUTPUT_DLLS)

clean-handlers: 
	@rm -f $(OUTPUT_DLLS)
	@echo [*] Cleaned DLL Files

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
	@rm -f $(OBJ) $(OUTPUT) $(OUTPUT_DLLS)
	@echo [*] Cleaned All Files

header: 
	@echo ====================
	@echo Starting Compilation
	@echo ====================

footer:
	@echo ==================================
	@echo Successfully Completed compilation
	@echo ==================================

