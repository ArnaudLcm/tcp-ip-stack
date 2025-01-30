AR  = ar
CC  = gcc
CXX = g++

SRC_DIR = src
INCL_DIR = include
OBJ_DIR = obj
OUT_DIR = bin

C_FILES = $(wildcard $(SRC_DIR)/*.c)
HEADER_FILES = $(wildcard $(INCL_DIR)/*.h)
OBJECT_FILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(C_FILES))
LIB_FILES = $(OUT_DIR)/libtcpip.a

CFLAGS = -Wall -Wextra -g -pthread -fsanitize=address,undefined

EXAMPLE_LFLAGS = -ltcpip
EXAMPLE_1 = $(OUT_DIR)/example_1

EXAMPLE_CFILES_1 = examples/example_1.c
EXAMPLE_HFILES_1 = 

.PHONY: all clean examples run gdb

all: $(LIB_FILES) examples

examples: $(EXAMPLE_1)

clean:
	@rm -rf $(OUT_DIR) $(OBJ_DIR)
	@mkdir -p $(OUT_DIR) $(OBJ_DIR)

$(OBJ_DIR) $(OUT_DIR):
	@mkdir -p $(OBJ_DIR) $(OUT_DIR)

$(LIB_FILES): $(OBJECT_FILES) | $(OUT_DIR)
	$(AR) rcs $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADER_FILES) | $(OBJ_DIR)
	$(CC) -c -o $@ $< $(CFLAGS) -I$(INCL_DIR)

$(EXAMPLE_1): $(LIB_FILES) $(EXAMPLE_CFILES_1) $(EXAMPLE_HFILES_1)
	$(CC) -o $@ $(EXAMPLE_CFILES_1) $(CFLAGS) $(EXAMPLE_LFLAGS) -I$(INCL_DIR) -L$(OUT_DIR)

run: examples
	@echo "Running examples..."
	@$(EXAMPLE_1)

