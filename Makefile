FLAGS = -ansi -Wall -pedantic -std=c99
GCC=gcc
SRC=src

all:
			$(GCC) -o shell $(FLAGS) $(SRC)/main.c

clean:
			@rm -f `/bin/ls *.o | grep -v Makefile | grep -v $(SRC)`
