#ifndef _MONSTRE_H
#define _MONSTRE_H

typedef struct monstre{
    char* name; // Longueure variable demandée dans le sujet
    int pv, armure, force, vitesse_attaque, vitesse_deplacement;
    int xp;
}monstre;

monstre* creer_monstre();
void supprimer_monstre(monstre* m);


#define nbMonstreDifferents 2 // Nombre de variété de monstre différents
monstre gobelin = {"Gobelin", 10, 2, 3, 4, 5, 10};
monstre orc = {"Orc", 20, 5, 8, 2, 2, 25};
monstre* listMonstreExistant[nbMonstreDifferents] = {&gobelin, &orc};

#endif