OBJS	= src/editeur.o src/ncurses.o src/map.o
SOURCE	= src/editeur.c src/ncurses.c src/map.c
HEADER	= head/ncurses.h head/map.h
OUT	= editeur
CC	 = gcc
FLAGS	 = -c -Wall -O3 -Werror
LFLAGS	 = -lncursesw

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

bin/editeur.o: src/editeur.c
	$(CC) $(FLAGS) src/editeur.c

bin/ncurses.o: src/ncurses.c
	$(CC) $(FLAGS) src/ncurses.c 

bin/map.o: src/map.c
	$(CC) $(FLAGS) src/map.c 

clean:
	rm -f $(OBJS) $(OUT)