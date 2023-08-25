CC = gcc
CFLAGS = -W -Wall -g -Wextra -Werror
SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c,obj/%.o,$(SRC)) 
OBJDIR = ./obj
AOUT = my_eval
INCLUDES = include/*.h

all : $(AOUT)

my_eval : $(OBJ)
	$(CC) $(CFLAGS) -o $(AOUT) $(OBJ)

$(OBJ): | $(OBJDIR)

$(OBJDIR)/%.o : src/%.c
	@echo $<
	$(CC) $(CFLAGS) -c -o $@ $<

cleangnu :
	@rm ./gnuplot/*

clean:
	@rm ./obj/*.o
	@rm $(AOUT)

archive: 
	tar zcvf DIVRIOTIS_Constantin.tar.gz include/* src/* obj/ gnuplot/ matrice_adjacence.txt trace2650.txt sujet.pdf Makefile rapport*
