OBJS	= editeur.o map.o ncurses.o player.o artefact.o monstre.o serveur.o worldMap.o client.o
OUT	= editeur, serveur, client

OBJS0	= editeur.o map.o ncurses.o player.o artefact.o monstre.o
SOURCE0	= editeur.c map.c ncurses.c player.c artefact.c monstre.c
HEADER0	= map.h ncurses.h player.h artefact.h monstre.h
OUT0	= editeur

OBJS1	= serveur.o map.o player.o artefact.o monstre.o worldMap.o
SOURCE1	= serveur.c map.c player.c artefact.c monstre.c worldMap.c
HEADER1	= map.h player.h artefact.h monstre.h worldMap.h requete.h
OUT1	= serveur

OBJS2	= client.o map.o ncurses.o player.o artefact.o monstre.o
SOURCE2	= client.c map.c ncurses.c player.c artefact.c monstre.c
HEADER2	= requete.h map.h ncurses.h player.h artefact.h monstre.h
OUT2	= client

CC	 = gcc
FLAGS	 = -g -c -Wall -Werror
LFLAGS	 = -lncursesw
LFLAGS2  = -lpthread

#all: editeur serveur client cleanOBJ
all: editeur serveur client

editeur: $(OBJS0) $(LFLAGS)
	$(CC) -g $(OBJS0) -o $(OUT0) $(LFLAGS)

serveur: $(OBJS1) $(LFLAGS)
	$(CC) -g $(OBJS1) -o $(OUT1)

client: $(OBJS2) $(LFLAGS)
	$(CC) -g $(OBJS2) -o $(OUT2) $(LFLAGS)

editeur.o: editeur.c
	$(CC) $(FLAGS) editeur.c 

map.o: map.c
	$(CC) $(FLAGS) map.c 

ncurses.o: ncurses.c
	$(CC) $(FLAGS) ncurses.c 

player.o: player.c
	$(CC) $(FLAGS) player.c 

artefact.o: artefact.c
	$(CC) $(FLAGS) artefact.c 

monstre.o: monstre.c
	$(CC) $(FLAGS) monstre.c 

serveur.o: serveur.c
	$(CC) $(FLAGS) serveur.c 

worldMap.o: worldMap.c
	$(CC) $(FLAGS) worldMap.c 

client.o: client.c
	$(CC) $(FLAGS) client.c 


clean:
	rm -f $(OBJS) $(OUT)

cleanOBJ:
	rm -f $(OBJS)