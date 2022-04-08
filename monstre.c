#include "monstre.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define nbMonstreDifferents 2 // Nombre de variété de monstre différents
monstre gobelin = {"Gobelin", 10, 2, 3, 4, 5, 10};
monstre orc = {"Orc", 20, 5, 8, 2, 2, 25};
monstre* listMonstreExistant[nbMonstreDifferents] = {&gobelin, &orc};

monstre* creer_monstre(){
    monstre* m = malloc(sizeof(monstre));
    srand(time(NULL));
    int nb = rand() % nbMonstreDifferents;
    monstre* ref = listMonstreExistant[nb];
    m->name = malloc(sizeof(char)*strlen(ref->name));
    strcpy(m->name, ref->name);
    m->pv = ref->pv;
    m->armure = ref->armure;
    m->force = ref->force;
    m->vitesse_attaque = ref->vitesse_attaque;
    m->vitesse_deplacement = ref->vitesse_deplacement;
    m->xp = ref->xp;
    return m;
}

void supprimer_monstre(monstre* m){
    free(m->name);
    free(m);
}