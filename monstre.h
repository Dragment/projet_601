#ifndef _MONSTRE_H
#define _MONSTRE_H

typedef struct monstre{
    char* name; // Longueur variable demandée dans le sujet
    int pv, armure, force, vitesse_attaque, vitesse_deplacement;
    int xp;
    int posX, posY;
}monstre;

monstre* creer_monstre(int, int);
void supprimer_monstre(monstre* m);

#endif