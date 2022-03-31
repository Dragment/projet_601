#ifndef _WORLDMAP_H
#define _WORLDMAP_H

#include "map.h"
//#include "monstre.h"
//#include "artefact.h"

// Listes Chainées
typedef struct{
    monstre* monstre;
    listMonstre* suiv;
}listMonstre;

typedef struct{
    artefact* artefact;
    listArtefact* suiv;
}listArtefact;

typedef struct{
    player* player;
    listPlayer* suiv;
}listPlayer;


// Map complete avec monstre et artefact
typedef struct completeMap{
    map* map;
    int x, y; // Coordonné par raport à la map de spawn

    // Listes chainées des entités
    listMonstre* listMonstreTete;
    listMonstre* listMonstreQueue;

    listArtefact* listArtefactTete;
    listArtefact* listArtefactQueue;

    listPlayer* listPlayerTete;
    listPlayer* listPlayerQueue;

    // Partie pour la liste chainée de worldMapList
    completeMap* suiv;

}completeMap;

// Liste de toute les maps générées
typedef struct worldMapList{
    char* repertoireCarte;
    completeMap* tete;
    completeMap* queue;
}worldMapList;


// Génère une map complete
completeMap* generer_complete_map(int x, int y, char* repertoire);
// Detruire complete map
void delete_complete_map(completeMap* m);
// Ajout dans les listes
void ajouter_monstre(completeMap* m, monstre* monstre);
void ajouter_artefact(completeMap* m, artefact* a);
// Supprimer les listes
void supprimer_list_monstre(completeMap* m);
void supprimer_list_artefact(completeMap* m);
void supprimer_list_player(completeMap* m);


// Initialise la worldMap et génère la map 0, 0
worldMapList init_world_map(char* repertoire);
// Détruire la worldMap
void delete_world_map(worldMapList m);
// Récupérer completeMap x, y ou la créer si elle n'existe pas encore
completeMap* get_or_create_complete_map(worldMapList m ,int x, int y);

#endif