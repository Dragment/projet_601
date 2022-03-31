#include "monstre.h"
#include <stdlib.h>



monstre* creer_monstre(){
    monstre* m = malloc(sizeof(monstre));
    srand(time(NULL));
    int nb = rand() % nbMonstreDifferents;
    monstre* ref = listMonstreExistant[nb];
    m->name = strcpy(ref->name);
    m->pv = ref->pv;
    m->armure = ref->armure;
    m->force = ref->force;
    m->vitesse_attaque = ref->vitesse_attaque;
    m->vitesse_deplacement = ref->vitesse_deplacement;
    m->xp = ref->xp;
    return m;
}

void supprimer_monstre(monstre* m){
    free(m);
}