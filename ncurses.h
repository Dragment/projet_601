#ifndef _NCURSES_H
#define _NCURSES_H

#include "autoload.h"

// Couleurs
#define BLEU 1
#define ROUGE 2
#define VERT 3
#define JAUNE 4
#define MAGENTA 5
#define CYAN 6
#define BLANC 7
#define NOIR 8

#define BG_BLEU 9
#define BG_ROUGE 10
#define BG_VERT 11
#define BG_JAUNE 12
#define BG_MAGENTA 13
#define BG_CYAN 14
#define BG_BLANC 15
#define BG_NOIR 16

// Méthodes
void ncurses_init();
void ncurses_stop();
void ncurses_couleurs();
void palette();
void ncurses_initsouris();
int souris_getpos(int *x, int *y);

#endif