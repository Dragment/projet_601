#ifndef _MONSTRE_H
#define _MONSTRE_H

#include <pthread.h>

typedef struct monstre{
    char* name; // Longueur variable demandée dans le sujet
    pthread_t threadId;
    int pv, armure, force, vitesse_attaque, vitesse_deplacement;
    int xp;
    int posX, posY;
}monstre;

monstre* creer_monstre(int, int);
void supprimer_monstre(monstre* m);

#endif