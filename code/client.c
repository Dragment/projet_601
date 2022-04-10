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
    }else if(strcmp(item, "player") == 0){
        return 'P';
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
    }else if(strcmp(item, "player") == 0){
        return MAP_PLAYER;
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
    case(MAP_PLAYER):
        return "player";
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

void afficher_attributs(WINDOW* attributs, player p, char* nomJoueur, char listNomArtefact[5]){
    wclear(attributs);
    wprintw(attributs, "Joueur :\n%s\n\nPV : %d/%d\nArmure : %d\nForce : %d\nVit atq : %d\nVit dep : %d\n\nXP : %d/100\nPieces : %d\n\nArtefacts :\n", nomJoueur, p.pv, p.pvMax, p.armure, p.force, p.vitesse_attaque, p.vitesse_deplacement, p.xp, p.nbPieces);
    for(int i = 0; i < 5; i++){
        wprintw(attributs, "%s\n", getArtefactNameFromChar(listNomArtefact[i])); 
    }
    wrefresh(attributs);
}

typedef struct arg_actualiserMap{
    WINDOW* carte;
    WINDOW* attributs;
    int* map_x;
    int* map_y;
    int* playerId;
    map* m;
    player* p;
    char* nomJoueur;
    pthread_mutex_t mutex;
    int socket;
}arg_actualiserMap;

void* actualisationMap(void* arg){
    arg_actualiserMap* args = (arg_actualiserMap*)arg;
    requete requete;
    requete.playerId = *args->playerId;
    while(stop == 0){
        pthread_mutex_lock(&args->mutex);

        requete.map_x = *args->map_x;
        requete.map_y = *args->map_y;
        requete.commande = ACTUALISER;

        //Envoi requete
        if(write(args->socket, &requete, sizeof(requete))== -1) {
            perror("Erreur lors de l'envoi TCP ");
            exit(EXIT_FAILURE);
        }
        //Réponse
        reponse_map_et_player repmp;
        if(read(args->socket, &repmp, sizeof(reponse_map_et_player)) == -1) {
            perror("Erreur lors de la réception de la première map ");
            exit(EXIT_FAILURE);
        }

        *args->m = repmp.m;
        *args->p = repmp.p;

        dessinner_map(args->carte, args->m);
        afficher_attributs(args->attributs, *args->p, args->nomJoueur, repmp.listArtefact);

        pthread_mutex_unlock(&args->mutex);

        sleep(0.5);
    }

    pthread_exit(NULL);
}

int lancerJeu(int socket, char* nomJoueur){
    
    //Initialiser variables
    int map_x = 0; // Les identifiants de la map actuelle (coordonnées worldMap)
    int map_y = 0;
    int playerId = getpid(); // L'identifiant du player sera le pid du processus client
    
    map* m = initialiser_map_vide();

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
    reponse_map_et_player repmp;
    if(read(socket, &repmp, sizeof(reponse_map_et_player)) == -1) {
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
    player p = repmp.p;
    afficher_attributs(attributs, p, nomJoueur, repmp.listArtefact);

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
    *m = repmp.m;
    dessinner_map(carte, m);

    // Lancer thread actualisation
    arg_actualiserMap arg;
    arg.map_x = &map_x;
    arg.map_y = &map_y;
    arg.carte = carte;
    arg.attributs = attributs;
    arg.m = m;
    arg.p = &p;
    arg.playerId = &playerId;
    arg.nomJoueur = nomJoueur;
    pthread_mutex_init(&arg.mutex, NULL);
    arg.socket = socket;

    pthread_t threadId;
    if(pthread_create(&threadId, NULL, actualisationMap, (void*)&arg) != 0){
        fprintf(stderr, "Erreur lancement thread actualisation.\n");
    }

    while((ch = getch()) && stop == 0) {
        // Les déplacements + espace
        if(ch == KEY_UP || ch == KEY_DOWN || ch == KEY_RIGHT || ch == KEY_LEFT || ch == ' '){
            pthread_mutex_lock(&arg.mutex);
            requete.map_x = map_x;
            requete.map_y = map_y;
            
            // PlayerId bouge pas
            requete.commande = MOVE;

            if(ch == KEY_UP){
                // Flèche haut
                requete.option = 'U';
            }else if(ch == KEY_DOWN){
                // Flèche bas
                requete.option = 'D';
            }else if(ch == KEY_RIGHT){
                // Flèche droite
                requete.option = 'R';
            }else if(ch == KEY_LEFT){
                // Flèche gauche
                requete.option = 'L';
            }else if(ch == ' '){
                // Espace
                requete.option = 'S';
            }
        }else if(ch == '1' || ch == '2' || ch == '3' || ch =='4' || ch == '5'){
            pthread_mutex_lock(&arg.mutex);
            requete.map_x = map_x;
            requete.map_y = map_y;
            
            requete.commande = DROP_ARTEFACT;
            if(ch == '1'){
                requete.option = '1';
            }else if(ch == '2'){
                requete.option = '2';
            }else if(ch == '3'){
                requete.option = '3';
            }else if(ch == '4'){
                requete.option = '4';
            }else if(ch == '5'){
                requete.option = '5';
            }

        }
        //Envoi requete
        if(write(socket, &requete, sizeof(requete))== -1) {
            perror("Erreur lors de l'envoi TCP ");
            exit(EXIT_FAILURE);
        }
        //Réponse
        reponse_map_et_player repmp;
        if(read(socket, &repmp, sizeof(reponse_map_et_player)) == -1) {
            perror("Erreur lors de la réception de la première map ");
            exit(EXIT_FAILURE);
        }

        *m = repmp.m;
        *arg.p = repmp.p;

        pthread_mutex_unlock(&arg.mutex);
    }

    if(pthread_join(threadId, NULL) != 0){
        fprintf(stderr, "Erreur join thread actualisation.\n");
    }

    pthread_mutex_destroy(&arg.mutex);

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
    printf("Rentrez le nom du joueur : ");
    scanf("%20s", name);
    printf("\n");
    while (strlen(name) <= 0 && strlen(name) > 21){ // \0 n'est pas pris en compte par strlen() donc on vérifie seulement que le nom ait une taille de 20 caractères
        printf("Nom incorect, réessayez : ");
        scanf("%20s", name);
        printf("\n");
    }

    // Envoi du nom du joueur au serveur
    if(write(fd, name, sizeof(char)*21)== -1) {
        perror("Erreur lors de l'envoi du nom du joueur ");
        exit(EXIT_FAILURE);
    }

    int repPseudo;
    // Lecture de la réponse du serveur
    if(read(fd, &repPseudo, sizeof(int)) == -1) {
        perror("Erreur lors de la réception de la confirmation du choix du nom du joueur ");
        exit(EXIT_FAILURE);
    }
    if(repPseudo != 0){
        fprintf(stderr, "Erreur validation nom\n");
        if(close(fd) == -1) {
            perror("Erreur lors de la fermeture de la socket ");
            exit(EXIT_FAILURE);
        }
        exit(EXIT_FAILURE);
    }

    // Traitement
    lancerJeu(fd, name);

    // Envoyer message fermeture flux tcp
    requete requete;
    requete.playerId = getpid();
    requete.commande = DECONNEXION;
    if(write(fd, &requete, sizeof(requete))== -1) {
        perror("Erreur lors de l'envoi de déconnexion TCP ");
        exit(EXIT_FAILURE);
    }
    //Réponse
    int repDeco;
    if(read(fd, &repDeco, sizeof(int)) == -1) {
        perror("Erreur lors de la réception de la déconnexion TCP ");
        exit(EXIT_FAILURE);
    }

    // Fermeture de la socket
    if(close(fd) == -1) {
        perror("Erreur lors de la fermeture de la socket ");
        exit(EXIT_FAILURE);
    }


    printf("Fermeture du client\n");
    return EXIT_SUCCESS;
}