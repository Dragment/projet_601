#include "artefact.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define nbArtefactDifferents 2
artefact artefact_vie = {"Artefact de Vie", 3, 0, 0, 0, 0};
artefact artefact_force = {"Artefact de pouvoir", 0, 3, 0, 0, 0};
artefact* listArtefactExistant[nbArtefactDifferents] = {&artefact_vie, &artefact_force};

artefact* creer_artefact(){
    artefact* a = malloc(sizeof(artefact));
    srand(time(NULL));
    int nb = rand() % nbArtefactDifferents;
    artefact* ref = listArtefactExistant[nb];
    strcpy(a->name, ref->name);
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