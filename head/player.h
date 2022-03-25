#ifndef _PLAYER_H
#define _PLAYER_H

#include "artfact.h"

typedef struct player{
    int pv, armure, force, vitesse_attaque, vitesse_deplacement;
    int nbPiece;
    artefact listArtefact[5];
}player;

#endif