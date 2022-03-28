#ifndef _MONSTRE_H
#define _MONSTRE_H

typedef struct monstre{
    char* name; // Longueure variable demandée dans le sujet
    int pv, armure, force, vitesse_attaque, vitesse_deplacement;
    int xp;
}monstre;

#endif