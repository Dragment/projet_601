#include <pthread.h>
#include "autoload.h"

int stop = 0; // Arreter le serveur
worldMapList worldMap; // Initialisé dans le thread main avant de lancer tout autre thread


// Signal ctrl + c
void handler(int signum) {
    int r;

    if(signum == SIGINT) {
        printf("Demande de fin reçue.\n");
        stop = 1;
    }
    
    do {
        r = waitpid(-1, NULL, WNOHANG);
    } while((r != -1) || (errno == EINTR));
}

// Représente le thread qui s'occupe d'un client
void* player_thread(void* arg){

    int sockclient = *(int*)arg;

    // Partie nom du joueur //
    char buffer[21];
    int repPseudo = 0;

    // Lecture de la requête du client
    if(read(sockclient, buffer, sizeof(char)*21) == -1) {
        perror("Erreur lors de la réception de la confirmation du choix du nom du joueur ");
        exit(EXIT_FAILURE);
    }

    // Envoi de la validation du nom
    if(write(sockclient, &repPseudo, sizeof(int))== -1) {
        perror("Erreur lors de l'envoi du nom du joueur ");
        exit(EXIT_FAILURE);
    }

    // Trouver ou placer le player et le placer
    int spawn_x, spawn_y;
    trouver_lieu_spawn(&worldMap, &spawn_x, &spawn_y);
    player* p = initNewPlayer(buffer, spawn_x, spawn_y);
    printf("Connexion de %s\n", p->nom);
    // Ajouter le joueur a la liste des joueurs de la map 0, 0
    ajouter_joueur(worldMap.tete, p);
    // Ajouter le player a la case (la place lui est réservée dans trouver_lieu_spawn)
    worldMap.tete->map->list_case[spawn_x][spawn_y].player = p;

    // Déclaration variables
    int player_map_x = 0;
    int player_map_y = 0;
    int playerId = -1; // Id temporaire, ce sera ensuite le pid

    //Boucle de traitement
    requete requeteClient;
    int demandeDeco = 0;
    while(stop == 0 && demandeDeco == 0){
        if(read(sockclient, &requeteClient, sizeof(requete)) == -1) {
            perror("Erreur lors de la demande TCP ");
            exit(EXIT_FAILURE);
        }

        // Premier envoie de carte
        if(requeteClient.commande == PREMIERE_DEMANDE_CARTE){
            playerId = requeteClient.playerId;
            printf("Envoie de la première map à %d\n", playerId);
            // marche pas si on fait juste => map* map = get_or_create_complete_map(worldMap, player_map_x, player_map_y)->map;
            map m = *(get_or_create_complete_map(&worldMap, player_map_x, player_map_y)->map);
            player tempP = *p;
            reponse_map_et_player rmp;
            rmp.m = m;
            rmp.p = tempP;
            if(write(sockclient, &rmp, sizeof(reponse_map_et_player)) == -1) {
                perror("Erreur lors de l'envoi de la première map ");
                exit(EXIT_FAILURE);
            }
        }else if(requeteClient.commande == MOVE){
            switch (requeteClient.option)
            {
            case 'U':
                playerMove(&worldMap, get_or_create_complete_map(&worldMap, player_map_x, player_map_y), p, 'U', &player_map_x, &player_map_y);
                break;
            case 'D':
                playerMove(&worldMap, get_or_create_complete_map(&worldMap, player_map_x, player_map_y), p, 'D', &player_map_x, &player_map_y);
                break;
            case 'R':
                playerMove(&worldMap, get_or_create_complete_map(&worldMap, player_map_x, player_map_y), p, 'R', &player_map_x, &player_map_y);
                break;
            case 'L':
                playerMove(&worldMap, get_or_create_complete_map(&worldMap, player_map_x, player_map_y), p, 'L', &player_map_x, &player_map_y);
                break;
            case ' ':
                /* code */ // TODO: Pièce du grand tout
                break;
            default:
                
                break;
            }
            // Réponse
            map m = *(get_or_create_complete_map(&worldMap, player_map_x, player_map_y)->map);
            player tempP = *p;
            reponse_map_et_player rmp;
            rmp.m = m;
            rmp.p = tempP;
            if(write(sockclient, &rmp, sizeof(reponse_map_et_player)) == -1) {
                perror("Erreur lors de l'envoi de la première map ");
                exit(EXIT_FAILURE);
            }
        }else if(requeteClient.commande == DECONNEXION){
            //TODO: Supprimer player et player sur map
            int rep = 1;
            if(write(sockclient, &rep, sizeof(int)) == -1) {
                perror("Erreur lors de la réponse de déco ");
                exit(EXIT_FAILURE);
            }
            demandeDeco = 1;
        }else if(requeteClient.commande == ACTUALISER){
            map m = *(get_or_create_complete_map(&worldMap, player_map_x, player_map_y)->map);
            player tempP = *p;
            reponse_map_et_player rmp;
            rmp.m = m;
            rmp.p = tempP;
            if(write(sockclient, &rmp, sizeof(reponse_map_et_player)) == -1) {
                perror("Erreur lors de l'envoi de la première map ");
                exit(EXIT_FAILURE);
            }
        }
    }

    // Fermeture de la socket de communication
    if(close(sockclient) == -1) {
        perror("Erreur lors de la fermeture de la socket de communication ");
        exit(EXIT_FAILURE);
    }

    pthread_exit(NULL);
}

int main(int argc, char* argv[]){

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
        fprintf(stderr, "Usage : %s port\n", argv[0]);
        fprintf(stderr, "Où :\n");
        fprintf(stderr, "  port : le numéro de port d'écoute du serveur\n");
        fprintf(stderr, "Où :\n");
        fprintf(stderr, "  répertoire : le répertoire des maps\n");
        exit(EXIT_FAILURE);
    }

    // Créer la structure pour la world map
    fprintf(stderr, "World map dans %s\n", argv[2]);
    worldMap = init_world_map(argv[2]); // Variable globale
    fprintf(stderr, "map OK\n");

    // Création TCP
    int fd;
    int sockclient;
    struct sockaddr_in adresse;

    // Création de la socket
    if((fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
        perror("Erreur lors de la création de la socket ");
        exit(EXIT_FAILURE);
    }

    // Création de l'adresse du serveur
    memset(&adresse, 0, sizeof(struct sockaddr_in));
    adresse.sin_family = AF_INET;
    adresse.sin_addr.s_addr = htonl(INADDR_ANY);
    adresse.sin_port = htons(atoi(argv[1]));

    // Nommage de la socket
    if(bind(fd, (struct sockaddr*)&adresse, sizeof(struct sockaddr_in)) == -1) {
        perror("Erreur lors du nommage de la socket ");
        exit(EXIT_FAILURE);
    }

    // Mise en mode passif de la socket
    if(listen(fd, 1) == -1) {
        perror("Erreur lors de la mise en mode passif ");
        exit(EXIT_FAILURE);
    }

    // Ajouter accept et traitement
    while(stop == 0){

        // Attente d'une connexion
        if((sockclient = accept(fd, NULL, NULL)) == -1) {
            if(errno != EINTR) {
                perror("Erreur lors de la demande de connexion ");
                exit(EXIT_FAILURE);
            }
        }

        // Créer le thread joueur

        pthread_t threadId;
        void* arg = (void*)&sockclient;
        if(pthread_create(&threadId, NULL, player_thread, arg) != 0){
            fprintf(stderr, "Erreur création thread joueur.\n");
        }
        if(pthread_detach(threadId) != 0){
            fprintf(stderr, "Erreur detach thread joueur.\n");
        }

    }

    // Fermeture de la socket de connexion
    if(close(fd) == -1) {
        perror("Erreur lors de la fermeture de la socket de connexion ");
        exit(EXIT_FAILURE);
    }

    // Delete worldMap
    delete_world_map(worldMap);

    printf("Fermeture du serveur\n");
    return EXIT_SUCCESS;
}