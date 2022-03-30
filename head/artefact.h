#ifndef _ARTEFACT_H
#define _ARTEFACT_H

typedef struct artefact{
    char* name; // Longueur variable demandée dans le sujet
    int pv, armure, force, vitesse_attaque, vitesse_deplacement;
}artefact;

artefact* creer_artefact();
void supprimer_artefact(artefact* a);

#define nbArtefactDifferents 2
artefact artefact_vie = {"Artefact de Vie", 3, 0, 0, 0, 0};
artefact artefact_force = {"Artefact de pouvoir", 0, 3, 0, 0, 0};
artefact* listArtefactExistant[nbArtefactDifferents] = {&artefact_vie, &artefact_force};

#endif