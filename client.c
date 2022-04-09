#include "autoload.h"

int stop = 0; // Arreter le serveur

// Signal ctrl + c
void handler(int signum) {
    int r;

    if(signum == SIGINT) {
        // printf("Demande de fin reçue.\n");
        stop = 1;
    }
    
    do {
        r = waitpid(-1, NULL, WNOHANG);
    } while((r != -1) || (errno == EINTR));
}

int verifDimTerminal(int xMin, int yMin){
    if(xMin <= COLS && yMin <= LINES){
        return 1;
    }else{
        return 0;
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

int lancerJeu(int socket){
    
    //Initialiser variables
    int map_x = 0; // Les identifiants de la map actuelle (coordonnées worldMap)
    int map_y = 0;
    int playerId = getpid(); // L'identifiant du player sera le pid du processus client
    
    map* m = initialiser_map_vide();
    // map* m;

    // Demander map au serveur
    requete requete;
    requete.map_x = map_x;
    requete.map_y = map_y;
    requete.playerId = playerId;
    requete.commande = PREMIERE_DEMANDE_CARTE;

    if(write(socket, &requete, sizeof(requete))== -1) {
        perror("Erreur lors de l'envoi de demande de première carte ");
        exit(EXIT_FAILURE);
    }

    // Lecture de la réponse du serveur
    if(read(socket, m, sizeof(map)) == -1) {
        perror("Erreur lors de la réception de la première map ");
        exit(EXIT_FAILURE);
    }

    // Initialisation ncurses
    ncurses_init();

    // Vérifier les dimensions du terminal
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
    WINDOW* cadreAttributs = newwin(22, 18, 12, 44);
    box(cadreAttributs, 0, 0);
    mvwprintw(cadreAttributs, 0, 1, "Attributs");
    wrefresh(cadreAttributs);
    // Sous fenetre 3
    WINDOW* attributs = derwin(cadreAttributs, 20, 16, 1, 1);
    //wprintw(fenetre2, "Fenetre");
    wrefresh(attributs);

    // Ajout des valeurs dans attributs
    //initialiser_attributs(attributs); //TODO: Initialiser affichage attributs
    wprintw(attributs, "Joueur : %s\n\nPV : %d/%d\nArmure : %d\nForce : %d\nVitesse d'attaque : %d\nVitesse de déplacement : %d\n\nXP : %d/100\nPièces possédées : %d\n\nStats d'artéfacts : %s", 
    /* pseudo, pv, pvMax, armure, force, vitesse d'attaque, vitesse de deplacement,
    xp, nbPieces, statsArtefact1, statsArtefact2, statsArtefact3, statsArtefact4, statsArtefact5*/);

    // Traitement des actions
    int ch;
    // int x, y;
    // char* background = ""; 
    // char* item = "";

    //Definide des couleurs à placer dans la map
    init_pair(100, COLOR_WHITE, COLOR_GREEN);
    init_pair(101, COLOR_WHITE, COLOR_YELLOW);
    init_pair(102, COLOR_WHITE, COLOR_BLUE);
    init_pair(103, COLOR_WHITE, COLOR_RED);
    init_pair(104, COLOR_WHITE, COLOR_BLACK);

    // Afficher la map chargée
    dessinner_map(carte, m);

    // Tant que q n'est pas apppuyé, on attends une saisie
    // while(((ch = getch()) != 'q') && stop == 0) {
    while((ch = getch()) && stop == 0) {
        // Les déplacements + espace
        if(ch == KEY_UP || ch == KEY_DOWN || ch == KEY_RIGHT || ch == KEY_LEFT || ch == ' '){
            requete.map_x = map_x;
            requete.map_y = map_y;
            // PlayerId bouge pas
            requete.commande = MOVE;

            if(ch == KEY_UP){
                // Flèche haut
                requete.option[0] = 'U';
            }else if(ch == KEY_DOWN){
                // Flèche bas
                requete.option[0] = 'D';
            }else if(ch == KEY_RIGHT){
                // Flèche droite
                requete.option[0] = 'R';
            }else if(ch == KEY_LEFT){
                // Flèche gauche
                requete.option[0] = 'L';
            }else if(ch == ' '){
                // Espace
                requete.option[0] = 'S';
            }
        }
        //Envoie requete
        if(write(socket, &requete, sizeof(requete))== -1) {
            perror("Erreur lors de l'envoi TCP ");
            exit(EXIT_FAILURE);
        }
        //Réponse
        int rep;
        // Lecture de la réponse du serveur
        if(read(socket, &rep, sizeof(int)) == -1) {
            perror("Erreur reception TCP ");
            exit(EXIT_FAILURE);
        }
        if(rep == 1){ //TODO: Vérifier si on laisse ce retour
            wprintw(info, "Action impossible\n");
            wrefresh(info);
        }
    }

    // Quitter ncurses
    ncurses_stop();

    // Libérer map
    supprimer_map(m);

    return EXIT_SUCCESS;
}


int main(int argc, char *argv[]) {
    int fd;
    struct sockaddr_in adresse;

    // Positionnement du gestionnaire (Signal ctrl+c)
    struct sigaction action;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    action.sa_handler = handler;
    if(sigaction(SIGCHLD, &action, NULL) == -1) {
        perror("Erreur lors du placement du gestionnaire ");
        exit(EXIT_FAILURE);    
    }
    if(sigaction(SIGINT, &action, NULL) == -1) {
        perror("Erreur lors du placement du gestionnaire ");
        exit(EXIT_FAILURE);    
    }
    char* buffer;
    
    // Vérification des arguments
    if(argc != 3) {
        fprintf(stderr, "Usage : %s adresseServeur portServeur\n", argv[0]);
        fprintf(stderr, "Où :\n");
        fprintf(stderr, "  adresseServeur : adresse IPv4 du serveur\n");
        fprintf(stderr, "  portServeur    : numéro de port du serveur\n");
        exit(EXIT_FAILURE);
    }

    // Création de la socket
    if((fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
        perror("Erreur lors de la création de la socket ");
        exit(EXIT_FAILURE);
    }
    
    // Remplissage de la structure
    memset(&adresse, 0, sizeof(struct sockaddr_in));
    adresse.sin_family = AF_INET;
    adresse.sin_port = htons(atoi(argv[2]));
    if(inet_pton(AF_INET, argv[1], &adresse.sin_addr.s_addr) != 1) {
        perror("Erreur lors de la conversion de l'adresse ");
        exit(EXIT_FAILURE);
    }

    // Connexion au serveur
    if(connect(fd, (struct sockaddr*)&adresse, sizeof(adresse)) == -1) {
        perror("Erreur lors de la connexion ");
        exit(EXIT_FAILURE);
    }

    // Choix du nom du joueur
    char name[21] = "";
    while (strlen(name) > 0 && strlen(name) < 21){ // \0 n'est pas pris en compte par strlen() donc on vérifie seulement que le nom ait une taille de 20 caractères
        printf("Rentrez le nom du joueur : ");
        scanf("%20s", name);
        printf("\n");
    }

    // Envoi du nom du joueur au serveur
    if(write(socket, buffer, sizeof(char)*21)== -1) {
        perror("Erreur lors de l'envoi du nom du joueur ");
        exit(EXIT_FAILURE);
    }

    // Lecture de la réponse du serveur
    if(read(socket, buffer, sizeof(char)*21) == -1) {
        perror("Erreur lors de la réception de la confirmation du choix du nom du joueur ");
        exit(EXIT_FAILURE);
    }

    // Traitement
    lancerJeu(fd);

    // Fermeture de la socket
    if(close(fd) == -1) {
        perror("Erreur lors de la fermeture de la socket ");
        exit(EXIT_FAILURE);
    }


    printf("Fermeture du client\n");
    return EXIT_SUCCESS;
}