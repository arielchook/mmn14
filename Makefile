CC = /usr/bin/gcc
CFLAGS = -g -ansi -Wall -pedantic -Wstrict-prototypes -Iinclude/ 
# uncomment to include the DEBUG define to get extra log messages
#CFLAGS = -g -ansi -Wall -pedantic -Wstrict-prototypes -Iinclude/ -DDEBUG=true

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
TEST_FILES := $(patsubst %.as,%,$(wildcard test/test_files/*.as))

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
	@echo "Cleaning up test results"
	@rm -f $(filter-out %.as,$(wildcard test/test_files/*))
	@echo "Running tests"
	@for test_file in $(TEST_FILES); do \
		echo "*********************** Running $$test_file ***********************"; \
		head -n 1 "$$test_file.as"; \
		echo "*****************************************************************************"; \
		bin/assembler $$test_file; \
	done

# Rule to clean up
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Phony targets
.PHONY: all clean test