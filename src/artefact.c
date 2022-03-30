#include "artefact.h"
#include <stdlib.h>

artefact* creer_artefact(){
    artefact* a = malloc(sizeof(artefact));
    srand(time(NULL));
    int nb = rand() % nbArtefactDifferents;
    artefact* ref = listArtefactExistant[nb];
    a->name = strcpy(ref->name);
    a->pv = ref->pv;
    a->armure = ref->armure;
    a->force = ref->force;
    a->vitesse_attaque = ref->vitesse_attaque;
    a->vitesse_deplacement = ref->vitesse_deplacement;
    return a;
}

void supprimer_artefact(artefact* a){
    free(a);
}