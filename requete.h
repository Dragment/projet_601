#ifndef _REQUETE_H
#define _REQUETE_H

// Liste des commandes
#define PREMIERE_DEMANDE_CARTE 1
#define MOVE 2

typedef struct requete{
    int map_x, map_y, playerId, commande;
    char option;
}requete;

#endif