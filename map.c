#include "autoload.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

map* initialiser_map_vide(){
    map* carte = malloc(sizeof(map));
    for(int i = 0; i<40; i++){
        for(int j = 0; j<20; j++){
            carte->list_case[i][j].x = i;
            carte->list_case[i][j].y = j;
            carte->list_case[i][j].element = MAP_VIDE;
            carte->list_case[i][j].background = MAP_GRASS;
        }
    }
    return carte;
}

void supprimer_map(map* m){
    free(m);
}

int creation_fichier(int nb, int* fd){
    char name[256];
    sprintf(name, "maps/%d.bin", nb);
    if((*fd = open(name, O_WRONLY|O_CREAT|O_EXCL, S_IRUSR|S_IWUSR)) == -1){
        return 0;
    }
    return 1;
}

int enregistrer_map(map* map){
    int fd = 0; // 0 Pour supprimer l'erreur de non initialisé alors que c'est initialisé via la fonction creation_fichier()
    
    // Choix du nom de sauvegarde
    int nb = 0;
    int retOuverture = 0;
    while(retOuverture != 1){
        retOuverture = creation_fichier(nb, &fd);
        nb++;
    }

    // Boucle sur chaque case de la map
    for(int i = 0; i<40; i++){
        for(int j = 0; j<20; j++){
            // Ecrire 4 fois un int
            if((write(fd, &(map->list_case[i][j].x), sizeof(int)) == -1)) {
                perror("Erreur d'écriture ");
                exit(EXIT_FAILURE);
            }
            if((write(fd, &(map->list_case[i][j].y), sizeof(int)) == -1)) {
                perror("Erreur d'écriture ");
                exit(EXIT_FAILURE);
            }
            if((write(fd, &(map->list_case[i][j].background), sizeof(int)) == -1)) {
                perror("Erreur d'écriture ");
                exit(EXIT_FAILURE);
            }
            if((write(fd, &(map->list_case[i][j].element), sizeof(int)) == -1)) {
                perror("Erreur d'écriture ");
                exit(EXIT_FAILURE);
            }
        }
    }
        
    // Fermer fichier
    if(close(fd) == -1) {
        perror("Erreur de fermeture");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}

int charger_map(map* map, int numMap){
    int fd;
    
    // Choix nom sauvegarde
    char name[256];
    sprintf(name, "maps/%d.bin", numMap);

    if((fd = open(name, O_RDONLY)) == -1){
        perror("Erreur ouverture fichier: ");
        exit(EXIT_FAILURE);
    }

    int pos;
    if((pos = lseek(fd, 0, SEEK_CUR)) == -1){
        perror("Erreur placement lseek: ");
        exit(EXIT_FAILURE);
    }
    //fprintf(stderr, "Position: %d\n", pos);

    // Boucle sur chaque case de la map
    for(int i = 0; i<40; i++){
        for(int j = 0; j<20; j++){
            // Lire 4 fois un int
            if(read(fd, &(map->list_case[i][j].x), sizeof(int)) == -1) {
                perror("Erreur read: ");
                exit(EXIT_FAILURE);
            }
            if(read(fd, &(map->list_case[i][j].y), sizeof(int)) == -1) {
                perror("Erreur read: ");
                exit(EXIT_FAILURE);
            }
            if(read(fd, &(map->list_case[i][j].background), sizeof(int)) == -1) {
                perror("Erreur read: ");
                exit(EXIT_FAILURE);
            }
            if(read(fd, &(map->list_case[i][j].element), sizeof(int)) == -1) {
                perror("Erreur read: ");
                exit(EXIT_FAILURE);
            }
        }
    }
        
    // Fermer fichier
    if(close(fd) == -1) {
        perror("Erreur de fermeture");
        exit(EXIT_FAILURE);
    }
    
    return EXIT_SUCCESS;
}

int enregistrer_edit_map(map* map, int numMap){
    int fd;
    
    // Choix du nom de sauvegarde
    char name[256];
    sprintf(name, "maps/%d.bin", numMap);

    if((fd = open(name, O_WRONLY)) == -1){
        perror("Erreur ouverture fichier: ");
        exit(EXIT_FAILURE);
    }

    // Boucle sur chaque case de la map
    for(int i = 0; i<40; i++){
        for(int j = 0; j<20; j++){
            // Ecrire 4 fois un int
            if((write(fd, &(map->list_case[i][j].x), sizeof(int)) == -1)) {
                perror("Erreur d'écriture ");
                exit(EXIT_FAILURE);
            }
            if((write(fd, &(map->list_case[i][j].y), sizeof(int)) == -1)) {
                perror("Erreur d'écriture ");
                exit(EXIT_FAILURE);
            }
            if((write(fd, &(map->list_case[i][j].background), sizeof(int)) == -1)) {
                perror("Erreur d'écriture ");
                exit(EXIT_FAILURE);
            }
            if((write(fd, &(map->list_case[i][j].element), sizeof(int)) == -1)) {
                perror("Erreur d'écriture ");
                exit(EXIT_FAILURE);
            }
        }
    }
        
    // Fermer fichier
    if(close(fd) == -1) {
        perror("Erreur de fermeture");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}