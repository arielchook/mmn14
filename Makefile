CC = /usr/bin/gcc
EXE = assembler
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = .
CFLAGS = -ansi -Wall -pedantic -Iinclude/

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))
TARGET = $(BIN_DIR)/$(EXE)

all: $(TARGET)
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)/*.o $(TARGET)

test:
	@echo Running tests..
	
