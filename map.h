#ifndef _MAP_H
#define _MAP_H

#define MAP_VIDE 0

#define MAP_PLAYER 1
#define MAP_MONSTER 2
#define MAP_TRESOR 3
#define MAP_ARTIFACT 4
#define MAP_OBSTACLE 5

#define MAP_GRASS 1
#define MAP_MONTAGNE 2
#define MAP_SAND 3
#define MAP_WATER 4

#define ELEMENT 0
#define BACKGROUND 1

#include "player.h"
#include "artefact.h"
#include "monstre.h"

typedef struct{
    int x, y; // Position de la case sur la map | x => 0 à 39 | y => 0, 19
    int element; // Ce qui est présent sur la case (vide / joueur / ennemi...)
    int background; // Le type de sol (herbre / montage / sable / eau)
    player* player; // NULL si pas de joueur sur la case
    monstre* monstre; // NULL si pas de monstre sur la case
    artefact* artefact; // NULL si pas d'artefact sur la case
}case_map;

typedef struct{
    case_map list_case[40][20];
}map;

map* initialiser_map_vide();
void supprimer_map(map*);
int creation_fichier(int, int*);
int enregistrer_map(map*);
int charger_map(map*, int);
int enregistrer_edit_map(map*, int);
char typeCase(map*, int, int);
int typeCaseDecompression(char, int);
// char* compresserMap(map*);
// void decompresserMap(char*, map*);

#endif