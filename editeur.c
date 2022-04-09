#include "autoload.h"

#define DEBUT_X = 1;
#define DEBUT_Y = 13;

int verifDimTerminal(int xMin, int yMin){
    if(xMin <= COLS && yMin <= LINES){
        return 1;
    }else{
        return 0;
    }
}

void initialiser_outils(WINDOW* outils){
    // Ajout des valeurs dans outils
    wprintw(outils, "\n   ");
    wattron(outils, COLOR_PAIR(BG_VERT));
    wprintw(outils, " ");
    wattroff(outils, COLOR_PAIR(BG_VERT));
    wprintw(outils, " Herbe\n   ");
    
    wattron(outils, COLOR_PAIR(BG_JAUNE));
    wprintw(outils, " ");
    wattroff(outils, COLOR_PAIR(BG_JAUNE));
    wprintw(outils, " Sable\n   ");

    wattron(outils, COLOR_PAIR(BG_BLEU));
    wprintw(outils, " ");
    wattroff(outils, COLOR_PAIR(BG_BLEU));
    wprintw(outils, " Eau\n   ");
    
    wattron(outils, COLOR_PAIR(BG_ROUGE));
    wprintw(outils, " ");
    wattroff(outils, COLOR_PAIR(BG_ROUGE));
    wprintw(outils, " Montagne\n  ");
    
    wprintw(outils, " X Obstacle\n  ");
    wprintw(outils, " $ Tresor\n  ");
    wprintw(outils, " M Monstre\n  ");
    wprintw(outils, " A Artefact\n  ");
    wrefresh(outils);
}

void choisirOutils(WINDOW* fenetre, int x, int y, char** background, char** item){
    // Choix de l'outils
    switch (y)
    {
    case 14: //Herbe
        if(strcmp(*background, "herbe")==0){
            *background = "";
            mvwprintw(fenetre, 1, 1, " ");
        }else{
            *background = "herbe";
            mvwprintw(fenetre, 1, 1, "x");
            mvwprintw(fenetre, 2, 1, " ");
            mvwprintw(fenetre, 3, 1, " ");
            mvwprintw(fenetre, 4, 1, " ");
        }
        wrefresh(fenetre);
        break;
    case 15: //Sable
        if(strcmp(*background, "sable")==0){
            *background = "";
            mvwprintw(fenetre, 2, 1, " ");
        }else{
            *background = "sable";
            mvwprintw(fenetre, 1, 1, " ");
            mvwprintw(fenetre, 2, 1, "x");
            mvwprintw(fenetre, 3, 1, " ");
            mvwprintw(fenetre, 4, 1, " ");
        }
        wrefresh(fenetre);
        break;
    case 16: //Eau
        if(strcmp(*background, "eau")==0){
            *background = "";
            mvwprintw(fenetre, 3, 1, " ");
        }else{
            *background = "eau";
            mvwprintw(fenetre, 1, 1, " ");
            mvwprintw(fenetre, 2, 1, " ");
            mvwprintw(fenetre, 3, 1, "x");
            mvwprintw(fenetre, 4, 1, " ");
        }
        wrefresh(fenetre);
        break;
    case 17: //Montagne
        if(strcmp(*background, "montagne")==0){
            *background = "";
            mvwprintw(fenetre, 4, 1, " ");
        }else{
            *background = "montagne";
            mvwprintw(fenetre, 1, 1, " ");
            mvwprintw(fenetre, 2, 1, " ");
            mvwprintw(fenetre, 3, 1, " ");
            mvwprintw(fenetre, 4, 1, "x");
        }
        wrefresh(fenetre);
        break;
    case 18: //Obstacle
        if(strcmp(*item, "obstacle")==0){
            *item = "";
            mvwprintw(fenetre, 5, 1, " ");
        }else{
            *item = "obstacle";
            mvwprintw(fenetre, 5, 1, "x");
            mvwprintw(fenetre, 6, 1, " ");
            mvwprintw(fenetre, 7, 1, " ");
            mvwprintw(fenetre, 8, 1, " ");
        }
        wrefresh(fenetre);
        break;
    case 19: //Tresor
        if(strcmp(*item, "tresor")==0){
            *item = "";
            mvwprintw(fenetre, 6, 1, " ");
        }else{
            *item = "tresor";
            mvwprintw(fenetre, 5, 1, " ");
            mvwprintw(fenetre, 6, 1, "x");
            mvwprintw(fenetre, 7, 1, " ");
            mvwprintw(fenetre, 8, 1, " ");
        }
        wrefresh(fenetre);
        break;
    case 20: //Monstre
        if(strcmp(*item, "monstre")==0){
            *item = "";
            mvwprintw(fenetre, 7, 1, " ");
        }else{
            *item = "monstre";
            mvwprintw(fenetre, 5, 1, " ");
            mvwprintw(fenetre, 6, 1, " ");
            mvwprintw(fenetre, 7, 1, "x");
            mvwprintw(fenetre, 8, 1, " ");
        }
        wrefresh(fenetre);
        break;
    case 21: //Artefact
        if(strcmp(*item, "artefact")==0){
            *item = "";
            mvwprintw(fenetre, 8, 1, " ");
        }else{
            *item = "artefact";
            mvwprintw(fenetre, 5, 1, " ");
            mvwprintw(fenetre, 6, 1, " ");
            mvwprintw(fenetre, 7, 1, " ");
            mvwprintw(fenetre, 8, 1, "x");
        }
        wrefresh(fenetre);
        break;
    default:
        break;
    }
}

int getBackgroundColor(char* background){
    if(strcmp(background, "herbe") == 0){
        return 100;
    }else if(strcmp(background, "sable") == 0){
        return 101;
    }else if(strcmp(background, "eau") == 0){
        return 102;
    }else if(strcmp(background, "montagne") == 0){
        return 103;
    }else{
        return 104;
    }
}

char getCharacter(char* item){
    if(strcmp(item, "obstacle") == 0){
        return 'X';
    }else if(strcmp(item, "tresor") == 0){
        return '$';
    }else if(strcmp(item, "monstre") == 0){
        return 'M';
    }else if(strcmp(item, "artefact") == 0){
        return 'A';
    }else{
        return ' ';
    }
}

void placer(WINDOW* fenetre, int x, int y, char* background, char* item){
    int color = getBackgroundColor(background);
    char character = getCharacter(item);
    
    wattron(fenetre, COLOR_PAIR(color));
    mvwprintw(fenetre, y-13, x-1, "%c", character);
    wattroff(fenetre, COLOR_PAIR(color));
    wrefresh(fenetre);
}

void remplir_herbe(WINDOW* fenetre){
    for(int x = 0; x<40; x++){
        for(int y = 0; y<20; y++){
            wattron(fenetre, COLOR_PAIR(BG_VERT));
            mvwprintw(fenetre, y, x, " ");
            wattroff(fenetre, COLOR_PAIR(BG_VERT));
        }
    }
    wrefresh(fenetre);
}

int getBackgroundInt(char* background){
    if(strcmp(background, "herbe") == 0){
        return MAP_GRASS;
    }else if(strcmp(background, "sable") == 0){
        return MAP_SAND;
    }else if(strcmp(background, "eau") == 0){
        return MAP_WATER;
    }else if(strcmp(background, "montagne") == 0){
        return MAP_MONTAGNE;
    }else{
        return MAP_VIDE;
    }
}

char getItemInt(char* item){
    if(strcmp(item, "obstacle") == 0){
        return MAP_OBSTACLE;
    }else if(strcmp(item, "tresor") == 0){
        return MAP_TRESOR;
    }else if(strcmp(item, "monstre") == 0){
        return MAP_MONSTER;
    }else if(strcmp(item, "artefact") == 0){
        return MAP_ARTIFACT;
    }else{
        return MAP_VIDE;
    }
}

void majMap(map* m, int x, int y, char* background, char* item){
    case_map* c = &(m->list_case[x-1][y-13]);
    c->background = getBackgroundInt(background);
    c->element = getItemInt(item);
}

char* intToStringBackground(int val){
    switch(val)
    {
    case (MAP_GRASS):
        return "herbe";
        break;
    case (MAP_MONTAGNE):
        return "montagne";
        break;
    case (MAP_SAND):
        return "sable";
        break;
    case (MAP_WATER):
        return "eau";
        break;
    default:
        return "VIDE";
        break;
    }
}

char* intToStringItem(int val){
    switch(val)
    {
    case(MAP_ARTIFACT):
        return "artefact";
        break;
    case(MAP_TRESOR):
        return "tresor";
        break;
    case(MAP_OBSTACLE):
        return "obstacle";
        break;
    case(MAP_MONSTER):
        return "monstre";
        break;
    default:
        return "VIDE";
        break;
    }
}

void dessinner_map(WINDOW* fenetre, map* map){
    char* bg;
    char* item;
    for(int x=0; x<40; x++){
        for(int y=0; y<20; y++){
            bg = intToStringBackground(map->list_case[x][y].background);
            item = intToStringItem(map->list_case[x][y].element);
            placer(fenetre, x+1, y+13, bg, item);
        }
    }
}

int creer_carte(){
    // Initialisation ncurses
    ncurses_init();

    if(verifDimTerminal(62, 36) == 0){
        ncurses_stop();
        fprintf(stderr, "Le terminal est trop petit pour lancer le jeu !\n");
        return EXIT_FAILURE;
    }

    // Initialiser la map
    map* map = initialiser_map_vide();

    // Fenetre 1
    WINDOW* cadreInfo = newwin(12, 62, 0, 0);
    box(cadreInfo, 0, 0); // Ajouter des bords à la fenetre
    mvwprintw(cadreInfo, 0, 1, "Informations");
    wrefresh(cadreInfo);
    // Sous fenetre 1
    WINDOW* info = derwin(cadreInfo, 10, 60, 1, 1);
    scrollok(info, TRUE);
    wrefresh(info);

    // Fenetre 2
    WINDOW* cadreCarte = newwin(22, 42, 12, 0);
    box(cadreCarte, 0, 0);
    mvwprintw(cadreCarte, 0, 1, "Carte");
    wrefresh(cadreCarte);
    // Sous fenetre 2
    WINDOW* carte = derwin(cadreCarte, 20, 40, 1, 1);
    //wprintw(fenetre2, "Fenetre");
    wrefresh(carte);

    // Fenetre 3
    WINDOW* cadreOutils = newwin(22, 18, 12, 44);
    box(cadreOutils, 0, 0);
    mvwprintw(cadreOutils, 0, 1, "Outils");
    wrefresh(cadreOutils);
    // Sous fenetre 3
    WINDOW* outils = derwin(cadreOutils, 20, 16, 1, 1);
    //wprintw(fenetre2, "Fenetre");
    wrefresh(outils);

    // Ajout des valeurs dans outils
    initialiser_outils(outils);

    // Traitement des actions
    int ch, x, y;
    char* background = ""; 
    char* item = "";

    //Definide des couleurs à placer dans la map
    init_pair(100, COLOR_WHITE, COLOR_GREEN);
    init_pair(101, COLOR_WHITE, COLOR_YELLOW);
    init_pair(102, COLOR_WHITE, COLOR_BLUE);
    init_pair(103, COLOR_WHITE, COLOR_RED);
    init_pair(104, COLOR_WHITE, COLOR_BLACK);
    init_pair(105, COLOR_BLACK, )

    // Remplir la map d'herbe
    remplir_herbe(carte);

    // Tant que q n'est pas apppuyé, on attends une saisie
    while((ch = getch()) != 'q') {
        // Si c'est un click souris
        if(ch==KEY_MOUSE){
            if(souris_getpos(&x, &y) == OK){
                // Dans la fenetre outils
                if(y>=13 && y<=35 && x>=45 && x<=60){
                    // Affichage dans la fenetre d'info
                    choisirOutils(outils, x, y, &background, &item);
                    wprintw(info, "Background: %s %s %s\n", background, " Item: ", item);
                    wrefresh(info);
                // Dans le fenetre map
                }else if(y>=13 && y<=32 && x>=1 && x<=40){
                    wprintw(info, "Placer Background: %s Item: %s en %d, %d\n", background, item, x, y);
                    wrefresh(info);
                    placer(carte, x, y, background, item);
                    // Mettre à jour map
                    majMap(map, x, y, background, item);
                // Sinon
                }else{
                    wprintw(info, "Pos x: %d / Pos y: %d => %d, %d\n", x, y);
                    wrefresh(info);
                }
            }
        }
    }

    // Pause
    // getch();

    // Quitter ncurses
    ncurses_stop();

    // Save map
    enregistrer_map(map);
    printf("Map enregistrée\n");

    // Libérer map
    supprimer_map(map);

    return EXIT_SUCCESS;
}

int editer_carte(){
    // Choix de la map à éditer
    int numMap;
    printf("Saisissez le numéro de la map à éditer: ");
    if(scanf("%d", &numMap) == EOF){
        perror("Erreur saisie: ");
        exit(EXIT_FAILURE);
    }

    // Charger map
    map* map = initialiser_map_vide();
    charger_map(map, numMap);

    // Initialisation ncurses
    ncurses_init();

    if(verifDimTerminal(62, 36) == 0){
        ncurses_stop();
        fprintf(stderr, "Le terminal est trop petit pour lancer le jeu !\n");
        return EXIT_FAILURE;
    }

    // Fenetre 1
    WINDOW* cadreInfo = newwin(12, 62, 0, 0);
    box(cadreInfo, 0, 0); // Ajouter des bords à la fenetre
    mvwprintw(cadreInfo, 0, 1, "Informations");
    wrefresh(cadreInfo);
    // Sous fenetre 1
    WINDOW* info = derwin(cadreInfo, 10, 60, 1, 1);
    scrollok(info, TRUE);
    wrefresh(info);

    // Fenetre 2
    WINDOW* cadreCarte = newwin(22, 42, 12, 0);
    box(cadreCarte, 0, 0);
    mvwprintw(cadreCarte, 0, 1, "Carte");
    wrefresh(cadreCarte);
    // Sous fenetre 2
    WINDOW* carte = derwin(cadreCarte, 20, 40, 1, 1);
    //wprintw(fenetre2, "Fenetre");
    wrefresh(carte);

    // Fenetre 3
    WINDOW* cadreOutils = newwin(22, 18, 12, 44);
    box(cadreOutils, 0, 0);
    mvwprintw(cadreOutils, 0, 1, "Outils");
    wrefresh(cadreOutils);
    // Sous fenetre 3
    WINDOW* outils = derwin(cadreOutils, 20, 16, 1, 1);
    //wprintw(fenetre2, "Fenetre");
    wrefresh(outils);

    // Ajout des valeurs dans outils
    initialiser_outils(outils);

    // Traitement des actions
    int ch, x, y;
    char* background = ""; 
    char* item = "";

    //Definide des couleurs à placer dans la map
    init_pair(100, COLOR_WHITE, COLOR_GREEN);
    init_pair(101, COLOR_WHITE, COLOR_YELLOW);
    init_pair(102, COLOR_WHITE, COLOR_BLUE);
    init_pair(103, COLOR_WHITE, COLOR_RED);
    init_pair(104, COLOR_WHITE, COLOR_BLACK);

    // Afficher la map chargée
    dessinner_map(carte, map);

    // Tant que q n'est pas apppuyé, on attends une saisie
    while((ch = getch()) != 'q') {
        // Si c'est un click souris
        if(ch==KEY_MOUSE){
            if(souris_getpos(&x, &y) == OK){
                // Dans la fenetre outils
                if(y>=13 && y<=35 && x>=45 && x<=60){
                    // Affichage dans la fenetre d'info
                    choisirOutils(outils, x, y, &background, &item);
                    wprintw(info, "Background: %s %s %s\n", background, " Item: ", item);
                    wrefresh(info);
                // Dans le fenetre map
                }else if(y>=13 && y<=32 && x>=1 && x<=40){
                    wprintw(info, "Placer Background: %s Item: %s en %d, %d\n", background, item, x, y);
                    wrefresh(info);
                    placer(carte, x, y, background, item);
                    // Mettre à jour map
                    majMap(map, x, y, background, item);
                // Sinon
                }else{
                    wprintw(info, "Pos x: %d / Pos y: %d => %d, %d\n", x, y);
                    wrefresh(info);
                }
            }
        }
    }

    // Pause
    // getch();

    // Quitter ncurses
    ncurses_stop();

    // Save map
    enregistrer_edit_map(map, numMap);
    printf("Map enregistrée\n");

    // Libérer map
    supprimer_map(map);

    return EXIT_SUCCESS;
}

int main(void){
    printf("Que voulez vous faire ?\n1 - Créer une nouvelle carte\n2 - éditer une carte\n");
    int choix;
    if(scanf("%d", &choix) == EOF){
        perror("Erreur saisie: ");
        exit(EXIT_FAILURE);
    }
    if(choix == 1){
        return creer_carte();
    }else if(choix == 2){
        return editer_carte();
    }else{
        printf("Choix impossible\n");
        return EXIT_FAILURE;
    }
}