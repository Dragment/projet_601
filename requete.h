#ifndef _REQUETE_H
#define _REQUETE_H

// Liste des commandes
#define PREMIERE_DEMANDE_CARTE 1
#define MOVE 2
#define DECONNEXION 3
#define ACTUALISER 4
#define DROP_ARTEFACT 5

typedef struct requete{
    int map_x, map_y, playerId, commande;
    char option;
}requete;

typedef struct reponse_map_et_player{
    map m;
    player p;
    char listArtefact[5];
}reponse_map_et_player;

#endif