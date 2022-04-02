#include "autoload.h"

int stop = 0; // Arreter le serveur

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
    worldMapList worldMap = init_world_map(argv[2]);

    // FIXME:
    delete_world_map(worldMap);

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
        }else{ // Sinon si la connexion réussie
            
            int pid;
            if((pid = fork()) == -1) {
                perror("Erreur lors de la création d'un fils ");
                exit(EXIT_FAILURE);
            }

            if(pid == 0){ // fils

                // Fermeture de la socket de connexion
                if(close(fd) == -1) {
                    perror("Erreur lors de la fermeture de la socket de connexion ");
                    exit(EXIT_FAILURE);
                }

                // Traitement
                // ... TODO:

                // Fermeture de la socket de communication
                if(close(sockclient) == -1) {
                    perror("Erreur lors de la fermeture de la socket de communication ");
                    exit(EXIT_FAILURE);
                }

                exit(EXIT_SUCCESS);

            }else{ // père

                // Fermeture de la socket de communication
                if(close(sockclient) == -1) {
                    perror("Erreur lors de la fermeture de la socket de communication ");
                    exit(EXIT_FAILURE);
                }

            }
        }
    }


    // Fermeture de la socket de connexion
    if(close(fd) == -1) {
        perror("Erreur lors de la fermeture de la socket de connexion ");
        exit(EXIT_FAILURE);
    }
}

