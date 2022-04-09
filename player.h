#ifndef _PLAYER_H
#define _PLAYER_H

#include "artefact.h"

typedef struct player{
    int pv, pvMax, armure, force, vitesse_attaque, vitesse_deplacement, nbPieces, xp;
    char* nom;
    artefact* listArtefact[5];
}player;

player* initNewPlayer(char*);
void upStats(player*);
void supprimer_player(player*);

#endif