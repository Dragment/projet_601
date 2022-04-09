#ifndef _REQUETE_H
#define _REQUETE_H

// Liste des commandes
#define PREMIERE_DEMANDE_CARTE 1

typedef struct requete{
    int map_x, map_y, playerId, commande;
}requete;

#endif