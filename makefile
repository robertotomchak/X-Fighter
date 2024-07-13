CC = gcc
CFLAGS = -g -Wall 

INCLUDE = ./include
LIB = ./lib
OBJ = ./obj

ALLEGRO = $(shell pkg-config allegro-5 allegro_main-5 allegro_primitives-5 allegro_image-5 allegro_font-5 allegro_ttf-5 --libs)

all: main

main: main.o libed | $(OBJ)
	$(CC) -o main $(OBJ)/*.o $(ALLEGRO)

libed: auxiliary.o fight_screen.o variable_bar.o player.o select_screen.o sprite.o bot.o

main.o: main.c | $(OBJ)
	$(CC) -c $(CFLAGS) $< -I $(INCLUDE) -o $(OBJ)/$@

%.o: $(LIB)/%.c $(INCLUDE)/%.h | $(OBJ)
	$(CC) -c $(CFLAGS) $< -I $(INCLUDE) -o $(OBJ)/$@

$(OBJ):
	mkdir $(OBJ)

clean:
	-rm -f $(OBJ)/*.o
 
purge: clean
	-rm -f main
