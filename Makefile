CC = /usr/bin/gcc
EXE = ./assembler
OBJS = *.o
CFLAGS = -ansi -Wall -pedantic 
INC=-Iinclude/
LD_FLAGS = 

all: $(EXE)

$(EXE): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LD_FLAGS)

$(OBJS): src/*.c

clean:
	-rm $(OBJS) $(EXE)

test:
	@echo Running tests..
	$(EXE) < test1.txt > test1_out.txt
	$(EXE) < test2.txt > test2_out.txt
	$(EXE) < test3.txt > test3_out.txt
	
%.o : %.c
	$(CC) -c $(INC) $(CFLAGS) $< 