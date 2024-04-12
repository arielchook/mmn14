CC = /usr/bin/gcc
CFLAGS = -g -ansi -Wall -pedantic -Wstrict-prototypes -Iinclude/ 

# Uncomment to include the DEBUG define to get extra log messages
#CFLAGS = -g -ansi -Wall -pedantic -Wstrict-prototypes -Iinclude/ -DDEBUG=true

# Directories
SRC_DIR := src
OBJ_DIR := obj
TEST_DIR := test
BIN_DIR := bin

# Find all source files except the one with main()
SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

# Find all test files
TEST_FILES := $(patsubst %.as,%,$(wildcard test/test*.as))

# Main target
all: assembler

# Compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Build the assembler
assembler: $(OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $(BIN_DIR)/$@

# Rule to create directories if they don't exist
$(OBJ_DIR) $(BIN_DIR):
	mkdir -p $@

# Run tests
test: assembler
	@echo "Cleaning up test results"
	@rm -f test/*.am test/*.ob test/*.ent test/*.ext
	@echo "Running tests for errornous scenarios"
	@for test_file in $(TEST_FILES); do \
		echo "*********************** Running $$test_file *********************************"; \
		head -n 1 "$$test_file.as"; \
		echo "*****************************************************************************"; \
		bin/assembler $$test_file; \
	done
	@echo "*****************************************************************************"
	@echo "************ Running test for valid scenario, and comparing output" *********"
	bin/assembler test/ps 
	diff -w test/ps.ob test/forum_files/ps.ob
	diff -w test/ps.ent test/forum_files/ps.ent
	diff -w test/ps.ext test/forum_files/ps.ext

# Rule to clean up
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Phony targets
.PHONY: all clean test