CC	 = gcc
FLAGS = -Wall -Werror
LFLAGS	 = -lncursesw

all: editeur

editeur: bin/editeur.o bin/ncurses.o bin/map.o
	$(CC) -o editeur bin/editeur.o bin/ncurses.o bin/map.o $(LFLAGS)

bin/editeur.o: src/editeur.c
	$(CC) -o bin/editeur.o -c src/editeur.c $(FLAGS)

bin/ncurses.o: src/ncurses.c
	$(CC) -o bin/ncurses.o -c src/ncurses.c $(FLAGS)

bin/map.o: src/map.c
	$(CC) -o bin/map.o -c src/map.c $(FLAGS)

clean:
	rm -rf bin/*.o