CC = gcc
CFLAGS = -Wall -g $(shellpkg-config --cflags)
LDFLAGS = $(shell pkg-config allegro-5 allegro_main-5 allegro_font-5 allegro_primitives-5 --libs)

	objects = auxiliary.o player.o main.o fight_screen.o

all: $(objects)
	gcc -o main $(objects) $(CFLAGS) $(LDFLAGS)

main.o: main.c
	$(CC) -c $(CFLAGS) main.c

auxiliary.o: auxiliary.c
	$(CC) -c $(CFLAGS) auxiliary.c

player.o: player.c
	$(CC) -c $(CFLAGS) player.c

fight_screen.o: fight_screen.c
	$(CC) -c $(CFLAGS) fight_screen.c

clean: rm -f $(objects)

purge: clean
	rm -f main
	