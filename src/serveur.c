#include "../head/autoload.h"

int main(int argc, char* argv[]){
    
    // Vérification des arguments
    if(argc != 2) {
        fprintf(stderr, "Usage : %s port\n", argv[0]);
        fprintf(stderr, "Où :\n");
        fprintf(stderr, "  port : le numéro de port d'écoute du serveur\n");
        exit(EXIT_FAILURE);
    }

    // Création TCP
    int fd;
    int sockclient;
    struct sockaddr_in adresse;
    struct sigaction action;

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
    int stop = 0;
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
                // ...

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

