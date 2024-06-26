CC = gcc
CFLAGS = -g -Wall 

INCLUDE = ./include
LIB = ./lib
OBJ = ./obj

ALLEGRO = $(shell pkg-config allegro-5 allegro_main-5 allegro_primitives-5 --libs)

all: main

main: main.o libed | $(OBJ)
	$(CC) -o main $(OBJ)/*.o $(ALLEGRO)

libed: auxiliary.o fight_screen.o health_bar.o player.o select_screen.o

main.o: main.c 
	$(CC) -c $(CFLAGS) $< -I $(INCLUDE) -o $(OBJ)/$@   

%.o: $(LIB)/%.c $(INCLUDE)/%.h
	$(CC) -c $(CFLAGS) $< -I $(INCLUDE) -o $(OBJ)/$@

$(OBJ):
	mkdir $(OBJ)

clean:
	-rm -f $(OBJ)/*.o
 
purge: clean
	-rm -f main
