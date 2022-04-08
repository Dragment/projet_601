#include "autoload.h"

int main(int argc, char *argv[]) {
    int fd;
    struct sockaddr_in adresse;
    
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

    // Traitement
    // ... TODO:
    char* n = "test";
    if(write(fd, n, sizeof(char)*4)== -1) {
        perror("Erreur lors de l'envoi de la valeur ");
        exit(EXIT_FAILURE);
    }
    printf("Client : valeur envoyée %s\n", n);

    // Lecture de la réponse du serveur
    char n2[2];
    if(read(fd, n2, sizeof(char)*2) == -1) {
        perror("Erreur lors de la réception de la valeur ");
        exit(EXIT_FAILURE);
    }
    printf("Client : valeur reçue %s.\n", n2);
    

    // Fermeture de la socket
    if(close(fd) == -1) {
        perror("Erreur lors de la fermeture de la socket ");
        exit(EXIT_FAILURE);
    }


    printf("Fermeture du client\n");
    return EXIT_SUCCESS;
}