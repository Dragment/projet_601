#include "artefact.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdio.h>

#define nbArtefactDifferents 2
artefact artefact_vie = {"Artefact de Vie", 3, 0, 0, 0, 0};
artefact artefact_force = {"Artefact de pouvoir", 0, 3, 0, 0, 0};
artefact* listArtefactExistant[nbArtefactDifferents] = {&artefact_vie, &artefact_force};

artefact* creer_artefact(){
    artefact* a = malloc(sizeof(artefact));
    srand(time(NULL));
    int nb = rand() % nbArtefactDifferents;
    // fprintf(stderr, "       Artefact: nbArtefact = %d\n", nb);
    artefact* ref = listArtefactExistant[nb];
    // fprintf(stderr, "       Artefact: strcpy / len(ref->name) = %ld\n", strlen(ref->name));
    a->name = malloc(sizeof(char)*strlen(ref->name));
    strcpy(a->name, ref->name);
    // fprintf(stderr, "       Artefact: strcpy ok\n");
    a->pv = ref->pv;
    a->armure = ref->armure;
    a->force = ref->force;
    a->vitesse_attaque = ref->vitesse_attaque;
    a->vitesse_deplacement = ref->vitesse_deplacement;
    return a;
}

void supprimer_artefact(artefact* a){
    free(a->name);
    free(a);
}