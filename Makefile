FLAGS = -ansi -Wall -pedantic -std=c99 -lm
GCC=gcc
SRC=src

all:
			$(GCC) -o shell $(FLAGS) $(SRC)/shell_final.c $ $(SRC)/parsing.c $(SRC)/pipe.c $(SRC)/touch.c $(SRC)/cat.c $(SRC)/cp.c $(SRC)/find.c

clean:
			rm shell
