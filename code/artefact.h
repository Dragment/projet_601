#ifndef _ARTEFACT_H
#define _ARTEFACT_H

typedef struct artefact{
    char* name; // Longueur variable demandée dans le sujet
    int pv, armure, force, vitesse_attaque, vitesse_deplacement;
}artefact;

artefact* creer_artefact();
void supprimer_artefact(artefact* a);
char getCharArtefact(artefact *a);
char* getArtefactNameFromChar(char c);



#endif