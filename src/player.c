#include "../head/player.h"

player* initNewPlayer(char* nom){
    player* p = (player*)malloc(sizeof(player));
    p->pvMax = 10;
    p->pv = p->pvMax;
    p->armure = 1;
    p->force = 1;
    p->vitesse_attaque = 1;
    p->vitesse_deplacement = 1;
    p->nbPiece = 0;
    p->xp = 0;
    p->nom = strcpy(nom);
    p->listArtefact = {NULL, NULL, NULL, NULL, NULL};
}

void upStats(player* p){
    int nb;
    srand(time(NULL));

    while (p->xp >= 100){
        nb = rand() % 5;
        switch(nb){
            case 0:
                p->armure++;
                break;
            case 1:
                p->pvMax += 10;
                p->pv += 10;
                break;
            case 2:
                p->force++;
                break;
            case 3:
                p->vitesse_attaque++;
                break;
            case 4:
                p->vitesse_deplacement++;
                break;
            default:
                break;
        }
        p->xp -= 100;
    }
}