#ifndef _PLAYER_H
#define _PLAYER_H

#include "artefact.h"

typedef struct player{
    int pv, pvMax, armure, force, vitesse_attaque, vitesse_deplacement, nbPieces, xp;
    int posX, posY;
    char* nom;
    artefact* listArtefact[5];
}player;

player* initNewPlayer(char*, int, int);
void upStats(player*);
void supprimer_player(player*);

#endif