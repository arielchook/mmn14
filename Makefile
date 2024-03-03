CC = gcc
EXE = ./assembler
OBJS = src/*.o
CFLAGS = -ansi -Wall -pedantic -Iinclude/
LD_FLAGS = -lm

all: $(EXE)

$(EXE): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LD_FLAGS)

clean:
	-rm $(OBJS) $(EXE)

test:
	@echo Running tests..
	$(EXE) < test1.txt > test1_out.txt
	$(EXE) < test2.txt > test2_out.txt
	$(EXE) < test3.txt > test3_out.txt
	
%.o : %.c
	$(CC) -c $(CFLAGS) $< 