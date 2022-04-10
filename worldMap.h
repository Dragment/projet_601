#ifndef _WORLDMAP_H
#define _WORLDMAP_H

#include <pthread.h>
#include "map.h"
#include "monstre.h"
#include "artefact.h"

// Listes Chainées

typedef struct completeMap completeMap;

typedef struct monsterMove_arg{
    completeMap* m;
    monstre* mon;
}monsterMove_arg;

typedef struct listMonstre{
    monstre* monstre;
    monsterMove_arg args;
    struct listMonstre* suiv;
}listMonstre;

typedef struct listArtefact{
    artefact* artefact;
    struct listArtefact* suiv;
}listArtefact;

typedef struct listPlayer{
    player* player;
    struct listPlayer* suiv;
}listPlayer;


// Map complete avec monstre et artefact
struct completeMap{
    map* map;
    int x, y; // Coordonnées par rapport à la map globale de spawn
    pthread_mutex_t mutex;


    // Listes chainées des entités
    listMonstre* listMonstreTete;
    listMonstre* listMonstreQueue;

    listArtefact* listArtefactTete;
    listArtefact* listArtefactQueue;

    listPlayer* listPlayerTete;
    listPlayer* listPlayerQueue;

    // Partie pour la liste chainée de worldMapList
    struct completeMap* suiv;

};

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
void ajouter_joueur(completeMap* m, player* p);
// Supprimer item
void supprimer_artefact_complete_map(completeMap* m, artefact* a);
void supprimer_monstre_complete_map(completeMap* m, monstre* mon);
void supprimer_player_complete_map(completeMap* m, player* p);
// Supprimer les listes
void supprimer_list_monstre(completeMap* m);
void supprimer_list_artefact(completeMap* m);
void supprimer_list_player(completeMap* m);


// Initialise la worldMap et génère la map 0, 0
worldMapList init_world_map(char* repertoire);
// Détruire la worldMap
void delete_world_map(worldMapList m);
// Récupérer completeMap x, y ou la créer si elle n'existe pas encore
completeMap* get_or_create_complete_map(worldMapList* m ,int x, int y);
void trouver_lieu_spawn(worldMapList* m, int* ret_x, int* ret_y);

// Mouvement
void playerMove(worldMapList* wm, completeMap* m, player* p, char mv, int* map_x_player, int* map_y_player);
void monsterMove(completeMap* m, monstre* monster);

void* monsterMove_routine(void* arg);

void pve(player* p, monstre* m);
void pvp(player* p, player* m);

void mort_player_world_map(completeMap* m, int x, int y);

#endif