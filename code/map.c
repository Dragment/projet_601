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

/*char typeCase(map* m, int x, int y){
    char c = "";
    
    switch(m->list_case[x][y].element){
        case MAP_ARTIFACT:
            if (m->list_case[x][y].background == MAP_GRASS) c = "A";
            else if (m->list_case[x][y].background == MAP_SAND) c = "B";
            else if (m->list_case[x][y].background == MAP_MONTAGNE) c = "C";
            else if (m->list_case[x][y].background == MAP_WATER) c = "D";
            break;
        case MAP_TRESOR:
            if (m->list_case[x][y].background == MAP_GRASS) c = "E";
            else if (m->list_case[x][y].background == MAP_SAND) c = "F";
            else if (m->list_case[x][y].background == MAP_MONTAGNE) c = "G";
            else if (m->list_case[x][y].background == MAP_WATER) c = "H";
            break;
        case MAP_PLAYER:
            if (m->list_case[x][y].background == MAP_GRASS) c = "I";
            else if (m->list_case[x][y].background == MAP_SAND) c = "J";
            else if (m->list_case[x][y].background == MAP_MONTAGNE) c = "K";
            else if (m->list_case[x][y].background == MAP_WATER) c = "L";
            break;
        case MAP_MONSTER:
            if (m->list_case[x][y].background == MAP_GRASS) c = "M";
            else if (m->list_case[x][y].background == MAP_SAND) c = "N";
            else if (m->list_case[x][y].background == MAP_MONTAGNE) c = "O";
            else if (m->list_case[x][y].background == MAP_WATER) c = "P";
            break;
        case MAP_OBSTACLE:
            if (m->list_case[x][y].background == MAP_GRASS) c = "Q";
            else if (m->list_case[x][y].background == MAP_SAND) c = "R";
            else if (m->list_case[x][y].background == MAP_MONTAGNE) c = "S";
            else if (m->list_case[x][y].background == MAP_WATER) c = "T";
            break;
        case MAP_VIDE:
            if (m->list_case[x][y].background == MAP_GRASS) c = "U";
            else if (m->list_case[x][y].background == MAP_SAND) c = "V";
            else if (m->list_case[x][y].background == MAP_MONTAGNE) c = "W";
            else if (m->list_case[x][y].background == MAP_WATER) c = "X";
            break;
        default:
            break;
    }
    
    return c;
}

int typeCaseDecompression(char c, int type){
    if (type == ELEMENT){
        if (c == "A" || c == "B" || c == "C" || c == "D")       return MAP_ARTIFACT;
        else if(c == "E" || c == "F" || c == "G" || c == "H")   return MAP_TRESOR;
        else if(c == "I" || c == "J" || c == "K" || c == "L")   return MAP_PLAYER;
        else if(c == "M" || c == "N" || c == "O" || c == "P")   return MAP_MONSTER;
        else if(c == "Q" || c == "R" || c == "S" || c == "T")   return MAP_OBSTACLE;
        else if(c == "U" || c == "V" || c == "W" || c == "X")   return MAP_VIDE;
    }else if(type == BACKGROUND){
        if(c == "A" || c == "E" || c == "I" || c == "M" || c == "Q" || c == "U")        return MAP_GRASS;
        else if(c == "B" || c == "F" || c == "J" || c == "N" || c == "R" || c == "V")   return MAP_SAND;
        else if(c == "C" || c == "G" || c == "K" || c == "O" || c == "S" || c == "W")   return MAP_MONTAGNE;
        else if(c == "D" || c == "H" || c == "L" || c == "P" || c == "T" || c == "X")   return MAP_WATER;
    }else{
        fprintf(stderr, "Type incorrect ");
        exit(EXIT_FAILURE);
    }
}

char* compresserMap(map* m){    // Permet de compresser l'affichage d'une map jusqu'à 801 octets (soit 20 maps au minimum dans une file de messages)
    char mStr[801] = NULL;
    char tmp = "", c = "";
    int compt = 0;

    // Parcourir la fenêtre
    for (int x = 0 ; x < 40 ; i++){
        for (int y = y < 20 ; y++){
            // Attribution d'un code à chaque case de type élément/background possible
            if (strcmp(tmp, c) == 0){
                compt++;
            }else{
                if (compt > 1){
                    strcat(mStr, tmp);
                    strcat(mStr, compt);  
                }
                else strcat(mStr, tmp);

                compt = 0;
                tmp = c;
            }  
        }
    }

    if (strlen(mStr) < 801)
        strcat(mStr, '\0');

    return mStr;
}

void decompresserMap(char* mStr, map* m){   // Permet de reproduire le visuel d'une map complète
                                            // Non terminée car non utilisée pour le rendu final
    char c = "";    // Type des cases
    char* tmp = NULL;     // Permet de stocker les chiffres formant un nombre dans mStr temporairement
    char* tmp2 = NULL;
    int curr = 0, qte = 0, len = strlen(mStr);  // curr => indice du caractère traité
                                                // qte => nombre de cases du même type d'affilées
    int x = 0, y = 0;   // Coordonnées utilisées pour la map                                                                

    // Parcours de la chaîne
    while (curr < len){
        // On l'ajoute le caractère courant à la map en le changeant en plusieurs cases qui lui correspondent
        if(mStr[curr] >= "A" && mStr[curr] <= "X"){
            if (strcmp(tmp, NULL) != 0 && strcmp(tmp2, NULL) != 0){
                qte = atoi(tmp2);
                while (qte > 0){
                    m->list_case[x][y].element = typeCaseDecompression(tmp, ELEMENT);
                    m->list_case[x][y].background = typeCaseDecompression(tmp, BACKGROUND);
                    x = (x + 1) % 40;
                    y = (int) ((x + 1) / 20);
                    qte--;
                    tmp2 = NULL;
                }
            }
            tmp = mStr[curr];
        }else{
            strcat(tmp2, mStr[curr]);
        }
        curr++;
    }
}*/