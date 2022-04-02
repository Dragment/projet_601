#include "ncurses.h"

void ncurses_stop() {
    endwin();
}

void palette(){
    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(6, COLOR_CYAN, COLOR_BLACK);
    init_pair(7, COLOR_WHITE, COLOR_BLACK);
    init_pair(8, COLOR_BLACK, COLOR_BLACK);
    
    init_pair(9, COLOR_BLACK, COLOR_BLUE);
    init_pair(10, COLOR_BLACK, COLOR_RED);
    init_pair(11, COLOR_BLACK, COLOR_GREEN);
    init_pair(12, COLOR_BLACK, COLOR_YELLOW);
    init_pair(13, COLOR_BLACK, COLOR_MAGENTA);
    init_pair(14, COLOR_BLACK, COLOR_CYAN);
    init_pair(15, COLOR_BLACK, COLOR_WHITE);
    init_pair(16, COLOR_BLACK, COLOR_BLACK);        
}

void ncurses_couleurs() {
    if(has_colors() == FALSE) {
        ncurses_stop();
        fprintf(stderr, "Pas de gestion de la couleur pour ce terminal.\n");
    }else{
        start_color();
        palette();
    }
    
}

void ncurses_initsouris() {
    if(!mousemask(BUTTON1_PRESSED, NULL)) {
        ncurses_stop();
        fprintf(stderr, "Pas de gestion de la souris.\n");
    }
}

void ncurses_init() {
    initscr(); // Démarre le mode ncurses
    cbreak(); // Désactive la mise en buffer
    noecho(); // Désactive l'affichage des caractères saisis
    keypad(stdscr, TRUE); // Active les touches spécifiques (flèches)
    refresh(); // Met à jour l'affichage
    curs_set(FALSE); // Masque le curseur

    ncurses_couleurs(); // Active les couleurs
    ncurses_initsouris(); // Initialisation sourie
}

int souris_getpos(int *x, int *y) {
    MEVENT event;
    int resultat = getmouse(&event);
    if(resultat == OK) {
        *x = event.x; *y = event.y;
    }
    return resultat;
}