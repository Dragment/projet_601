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

typedef struct{
    int x, y; // Position de la case sur la map | x => 0 à 39 | y => 0, 19
    int element; // Ce qui est présent sur la case (vide / joueur / ennemi...)
    int background; // Le type de sol (herbre / montage / sable / eau)
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

#endif