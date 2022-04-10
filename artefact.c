#include "artefact.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdio.h>

#define nbArtefactDifferents 10
artefact artefact_vie = {"Artefact de Vie", 15, 0, 0, 0, 0};
artefact artefact_armure = {"Artefact d\'Armure", 0, 3, 0, 0, 0};
artefact artefact_force = {"Artefact de Force", 0, 0, 3, 0, 0};
artefact artefact_vit_att = {"Artefact de Vitesse d\'Attaque", 0, 0, 0, 3, 0};
artefact artefact_vit_dep = {"Artefact de Vitesse de Déplacement", 0, 0, 0, 0, 3};
artefact artefact_roi_rablite = {"Artefact du Roi-Rablite", 10, 10, 5, -3, 0};
artefact artefact_chef_delinquant = {"Artefact du Chef-Delinquant", -5, -2, 15, 3, 3};
artefact artefact_tigre = {"Artefact du Tigre", -5, 0, 3, 0, 3};
artefact artefact_fleur = {"Artefact de la Fleur", 5, -1, 2, 0, 0};
artefact artefact_bougre = {"Artefact du Bougre", -5, -5, -5, -5, -5};
artefact* listArtefactExistant[nbArtefactDifferents] = {&artefact_vie, &artefact_armure, &artefact_force, &artefact_vit_att, &artefact_vit_dep, &artefact_roi_rablite, &artefact_chef_delinquant, &artefact_tigre, &artefact_fleur, &artefact_bougre};
artefact* creer_artefact(){
    artefact* a = malloc(sizeof(artefact));
    srand(time(NULL));
    int nb = rand() % nbArtefactDifferents;
    // fprintf(stderr, "       Artefact: nbArtefact = %d\n", nb);
    artefact* ref = listArtefactExistant[nb];
    // fprintf(stderr, "       Artefact: strcpy / len(ref->name) = %ld\n", strlen(ref->name));
    a->name = malloc(sizeof(char)*strlen(ref->name));
    strcpy(a->name, ref->name);
    // fprintf(stderr, "       Artefact: strcpy ok\n");
    a->pv = ref->pv;
    a->armure = ref->armure;
    a->force = ref->force;
    a->vitesse_attaque = ref->vitesse_attaque;
    a->vitesse_deplacement = ref->vitesse_deplacement;
    return a;
}

void supprimer_artefact(artefact* a){
    free(a->name);
    free(a);
}

char getCharArtefact(artefact *a){
    if(a == NULL){
        return 'a';
    }else if(strcmp(a->name, "Artefact de Vie") == 0){
        return 'b';
    }else if(strcmp(a->name, "Artefact d\'Armure") == 0){
        return 'c';
    }else if(strcmp(a->name, "Artefact de Force") == 0){
        return 'd';
    }else if(strcmp(a->name, "Artefact de Vitesse d\'Attaque") == 0){
        return 'e';
    }else if(strcmp(a->name, "Artefact de Vitesse de Déplacement") == 0){
        return 'f';
    }else if(strcmp(a->name, "Artefact du Roi-Rablite") == 0){
        return 'g';
    }else if(strcmp(a->name, "Artefact du Chef-Delinquant") == 0){
        return 'h';
    }else if(strcmp(a->name, "Artefact du Tigre") == 0){
        return 'i';
    }else if(strcmp(a->name, "Artefact de la Fleur") == 0){
        return 'j';
    }else if(strcmp(a->name, "Artefact du Bougre") == 0){
        return 'k';
    }else{
        return 'a';
    }
}

char* getArtefactNameFromChar(char c){
    if(c == 'a'){
        return "---";
    }else if(c == 'b'){
        return "Artefact Vie";
    }else if(c == 'c'){
        return "Artefact Armure";
    }else if(c == 'd'){
        return "Artefact Force";
    }else if(c == 'e'){
        return "Artefact frappe rapide";
    }else if(c == 'f'){
        return "Artefact vitesse";
    }else if(c == 'g'){
        return "Roi-Rablite";
    }else if(c == 'h'){
        return "Chef-Delinquant";
    }else if(c == 'i'){
        return "Artefact Tigre";
    }else if(c == 'j'){
        return "Artefact Fleur";
    }else if(c == 'k'){
        return "Artefact Bougre";
    }else{
        return "---";
    }
}