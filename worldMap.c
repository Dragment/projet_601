#include "worldMap.h"
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <time.h>

void ajouter_monstre(completeMap* m, monstre* monstre){
    listMonstre* l = malloc(sizeof(listMonstre));
    l->monstre = monstre;
    l->suiv = NULL;
    if(m->listMonstreTete == NULL){
        m->listMonstreTete = l;
        m->listMonstreQueue = l;
    }else{
        m->listMonstreQueue->suiv = l;
        m->listMonstreQueue = l;
    }
}

void ajouter_artefact(completeMap* m, artefact* a){
    listArtefact* l = malloc(sizeof(listArtefact));
    l->artefact = a;
    l->suiv = NULL;
    if(m->listArtefactTete == NULL){
        m->listArtefactTete = l;
        m->listArtefactQueue = l;
    }else{
        m->listArtefactQueue->suiv = l;
        m->listArtefactQueue = l;
    }
}

void supprimer_list_monstre(completeMap* m){
    listMonstre* l = m->listMonstreTete;
    while(l != NULL){
        listMonstre* suiv = l->suiv;
        supprimer_monstre(l->monstre);
        free(l);
        l = suiv;
    }
}
void supprimer_list_artefact(completeMap* m){
    listArtefact* l = m->listArtefactTete;
    while(l != NULL){
        listArtefact* suiv = l->suiv;
        supprimer_artefact(l->artefact);
        free(l);
        l = suiv;
    }
}

void supprimer_list_player(completeMap* m){
    listPlayer* l = m->listPlayerTete;
    while(l != NULL){
        listPlayer* suiv = l->suiv;
        supprimer_player(l->player);
        free(l);
        l = suiv;
    }
}

completeMap* generer_complete_map(int x, int y, char* repertoire){
    completeMap* m = malloc(sizeof(completeMap));
    m->x = x;
    m->y = y;

    // Initialiser les listes
    m->listMonstreTete = NULL;
    m->listMonstreQueue = NULL;

    m->listArtefactTete = NULL;
    m->listArtefactQueue = NULL;

    m->listMonstreTete = NULL;
    m->listMonstreQueue = NULL;

    // Charger une map random dans la liste des cartes

    // Trouver le nombre de map dispo
    int nbMapDispo = 0;
    DIR* dirp;
    struct dirent* entry;
    dirp = opendir("path"); /* There should be error handling after this */
    while ((entry = readdir(dirp)) != NULL) {
        //if (entry->d_type == DT_REG) { /* If the entry is a regular file */

            nbMapDispo++; // FIXME: Verifier que le nb de fichier est bon

        //}
    }
    closedir(dirp);

    // Choisir un nb aléatoire pour la map
    srand(time(NULL));
    int nbMap = rand() % nbMapDispo;

    // Charger map depuis fichier
    m->map = initialiser_map_vide();
    charger_map(m->map, nbMap);

    // Remplire les listes artefact et monstre (si on créer la map pas de héro présent)
    // Pour chaque case de la map
    case_map* c;
    for(int y = 0; y < 20; y++){
        for(int x = 0; x < 40; x++){
            c = &(m->map->list_case[x][y]);
            // si monstre ou artefact le créer et le placer dans la case et dans la liste
            if(c->element == MAP_MONSTER){
                c->monstre = creer_monstre();
                ajouter_monstre(m, c->monstre);
            }else if(c->element == MAP_ARTIFACT){
                c->artefact = creer_artefact();
                ajouter_artefact(m, c->artefact);
            }
        }
    }
    return(m);
}

void delete_complete_map(completeMap* m){
    supprimer_list_monstre(m);
    supprimer_list_artefact(m);
    supprimer_list_player(m);

    supprimer_map(m->map);
    free(m);
}

worldMapList init_world_map(char* repertoire){
    // Générer la map 0, 0
    completeMap* cm = generer_complete_map(0, 0, repertoire);
    worldMapList m;
    m.repertoireCarte = NULL; // éviter erreur ‘m.repertoireCarte’ is used uninitialized in this function
    strcpy(m.repertoireCarte, repertoire);
    m.tete = cm;
    m.queue = cm;
    return(m);
}

void delete_world_map(worldMapList m){
    completeMap* curm = m.tete;
    completeMap* suiv;
    while (curm != NULL){
        suiv = curm->suiv;
        delete_complete_map(curm);
        curm = suiv;
    }
    m.tete = NULL;
    m.queue = NULL;
}

completeMap* get_or_create_complete_map(worldMapList m, int x, int y){
    completeMap* curm = m.tete;
    completeMap* suiv;
    completeMap* ret = NULL;
    while (curm != NULL && ret == NULL){
        suiv = curm->suiv;
        if(curm->x == x && curm->y == y){
            ret = curm;
        }
        curm = suiv;
    }
    if(ret == NULL){
        // CreerMap
        completeMap* newm = generer_complete_map(x, y, m.repertoireCarte);
        m.queue->suiv = newm;
        m.queue = newm;
        ret = newm; 
    }
    // Retourner map trouvée ou créée
    return ret;
}