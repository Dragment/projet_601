#include "player.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

player* initNewPlayer(char* nom, int posX, int posY){
    player* p = (player*)malloc(sizeof(player));
    p->pvMax = 25;
    p->pv = p->pvMax;
    p->armure = 10;
    p->force = 2;
    p->vitesse_attaque = 1;
    p->vitesse_deplacement = 1;
    p->nbPieces = 0;
    p->xp = 0;
    p->nom = malloc(sizeof(char)*strlen(nom));
    strcpy(p->nom, nom);
    for(int i = 0; i < 5; i++){
        p->listArtefact[i] = NULL;
    }
    p->posX = posX;
    p->posY = posY;
    return p;
}

void supprimer_player(player* p){
    free(p->nom);
    free(p);
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

int getNbArtefact(player* p){
    int nb = 0;
    for(int i = 0; i<5; i++){
        if(p->listArtefact[i] != NULL){
            nb++;
        }
    }
    return nb;
}

void ramasserArtefact(player* p, artefact* a){
    int pos = -1;
    for(int i = 0; i<5 && pos== -1; i++){
        if(p->listArtefact[i] == NULL){
            pos = i;
        }
    }
    if(pos != -1){
        // Mise a jour des attributs
        p->listArtefact[pos] = a;
        p->pvMax = p->pvMax + a->pv;
        if(p->pvMax < 1){
            p->pvMax = 1;
        }
        p->pv = p->pv + a->pv;
        if(p->pv < 1){
            p->pv = 1;
        }
        p->armure = p->armure + a->armure;
        if(p->armure < 1){
            p->armure = 1;
        }
        p->force = p->force + a->force;
        if(p->force < 1){
            p->force = 1;
        }
        p->vitesse_attaque = p->vitesse_attaque + a->vitesse_attaque;
        if(p->vitesse_attaque < 1){
            p->vitesse_attaque = 1;
        }
        p->vitesse_deplacement = p->vitesse_deplacement + a->vitesse_deplacement;
        if(p->vitesse_deplacement < 1){
            p->vitesse_deplacement = 1;
        }
    }
}

void ouvrirTresor(player* p){
    srand(time(NULL));
    int nb = rand()%3;
    // 1 chance sur 3 d'avoir une pièce
    if(nb == 0){
        p->nbPieces++;
        if(p->nbPieces > 3){
            p->nbPieces = 3;
        }
    }else{ // Sinon on a de la vie
        nb = (rand() %7) + 1; // Regagne entre 1 et 7 pv
        p->pv = p->pv + nb;
        if(p->pv > p->pvMax){
            p->pv = p->pvMax;
        }
    }
}

void reset_player(player* p, int posX, int posY){
    p->pvMax = 25;
    p->pv = p->pvMax;
    p->armure = 10;
    p->force = 2;
    p->vitesse_attaque = 1;
    p->vitesse_deplacement = 1;
    p->nbPieces = 0;
    p->xp = 0;
    for(int i = 0; i < 5; i++){
        p->listArtefact[i] = NULL;
    }
    p->posX = posX;
    p->posY = posY;
}