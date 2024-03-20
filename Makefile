CC = /usr/bin/gcc
CFLAGS = -ansi -Wall -pedantic -Iinclude/

# Directories
SRC_DIR := src
OBJ_DIR := obj
TEST_DIR := test
BIN_DIR := bin

# Find all source files except the one with main()
SRCS := $(filter-out $(SRC_DIR)/main.c, $(wildcard $(SRC_DIR)/*.c))
OBJS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

# Main source file
MAIN_SRC := $(SRC_DIR)/main.c

# Find all test source files
TEST_SRCS := $(wildcard $(TEST_DIR)/*.c)
TEST_BINS := $(patsubst $(TEST_DIR)/%.c,$(BIN_DIR)/%,$(TEST_SRCS))

# Main target
all: assembler $(TEST_BINS)

# Compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Build executables for test files
$(BIN_DIR)/%: $(TEST_DIR)/%.c $(OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@

# Build the assembler
assembler: $(MAIN_SRC) $(OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $(BIN_DIR)/$@

# Rule to create directories if they don't exist
$(OBJ_DIR) $(BIN_DIR):
	mkdir -p $@

# Run tests
test: $(TEST_BINS)
	echo "Running test_hashtable"
	./bin/test_hashtable
	echo "Running test_macros"
	./bin/test_macros ./test/ps1.as ./test/ps1.am
#	run diff to test the results ?
	echo "Running test_firstpass"
	./bin/test_firstpass ./test/fp1.am

#	@for test_bin in $(TEST_BINS); do \
#		echo "Running $$test_bin"; \
#		./$$test_bin; \
#	done

# Rule to clean up
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Phony targets
.PHONY: all clean test