#include "worldMap.h"
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <stdio.h>

void ajouter_joueur(completeMap* m, player* p){
    listPlayer* l = malloc(sizeof(listPlayer));
    l->player = p;
    l->suiv = NULL;
    if(m->listPlayerTete == NULL){
        m->listPlayerTete = l;
        m->listPlayerQueue = l;
    }else{
        m->listPlayerQueue->suiv = l;
        m->listPlayerQueue = l;
    }
}

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
    pthread_mutex_init(&m->mutex, NULL);    // TODO : Rajouter vérification "if ... != ..."

    // Initialiser les listes
    m->listMonstreTete = NULL;
    m->listMonstreQueue = NULL;

    m->listArtefactTete = NULL;
    m->listArtefactQueue = NULL;

    m->listMonstreTete = NULL;
    m->listMonstreQueue = NULL;

    // Charger une map random dans la liste des cartes

    // Trouver le nombre de map dispo
    // Enlever 2 pour dossier . et ./
    int nbMapDispo = -2;
    DIR* dirp;
    struct dirent* entry;
    dirp = opendir(repertoire);
    // fprintf(stderr, "CompleteMap: Go compter nb maps dans %s\n", repertoire);
    while ((entry = readdir(dirp)) != NULL) {
            nbMapDispo++;
    }
    // fprintf(stderr, "CompleteMap: Compter nb maps OK => %d\n", nbMapDispo);
    closedir(dirp);

    // Choisir un nb aléatoire pour la map
    srand(time(NULL));
    int nbMap = rand() % nbMapDispo;

    // fprintf(stderr, "CompleteMap: Map choisie %d\n", nbMap);

    // Charger map depuis fichier
    m->map = initialiser_map_vide();
    // fprintf(stderr, "CompleteMap: Go charger map\n");
    charger_map(m->map, nbMap);
    // fprintf(stderr, "CompleteMap: Chargement map ok \n");

    // Remplire les listes artefact et monstre (si on créer la map pas de héro présent)
    // Pour chaque case de la map
    // fprintf(stderr, "CompleteMap: charger monstre et artefact\n");
    case_map* c;
    for(int y = 0; y < 20; y++){
        for(int x = 0; x < 40; x++){
            c = &(m->map->list_case[x][y]);
            // si monstre ou artefact le créer et le placer dans la case et dans la liste
            if(c->element == MAP_MONSTER){
                // fprintf(stderr, "CompleteMap: monstre\n");
                c->monstre = creer_monstre(x, y); // TODO: On a modif la fonction, vérifier que ça marche toujours
                // fprintf(stderr, "   CompleteMap: creer monstre ok\n");
                ajouter_monstre(m, c->monstre);
                // fprintf(stderr, "   CompleteMap: ajout monstre ok\n");
            }else if(c->element == MAP_ARTIFACT){
                // fprintf(stderr, "CompleteMap: artefact\n");
                c->artefact = creer_artefact();
                // fprintf(stderr, "   CompleteMap: creer artefact ok\n");
                ajouter_artefact(m, c->artefact);
                // fprintf(stderr, "   CompleteMap: ajouter artefact ok\n");
            }
        }
    }
    // fprintf(stderr, "CompleteMap: chargement monstre artefact ok\n");
    return(m);
}

void delete_complete_map(completeMap* m){
    supprimer_list_monstre(m);
    supprimer_list_artefact(m);
    supprimer_list_player(m);
    pthread_mutex_destroy(&m->mutex);   // TODO : Rajouter vérification "if ... != ..."

    supprimer_map(m->map);
    free(m);
}

worldMapList init_world_map(char* repertoire){
    // Générer la map 0, 0
    // fprintf(stderr, "Init_world_map: Go init map\n");
    completeMap* cm = generer_complete_map(0, 0, repertoire);
    // fprintf(stderr, "Init_world_map: Complete map OK\n");
    worldMapList m;
    m.repertoireCarte = malloc(sizeof(char)*strlen(repertoire));
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
    free(m.repertoireCarte);
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

void trouver_lieu_spawn(worldMapList m, int* ret_x, int* ret_y){
    completeMap* cm = get_or_create_complete_map(m, 0, 0);
    srand(time(NULL));
    int testx, testy;
    testx = rand() %40;
    testy = rand() %20;
    pthread_mutex_lock(&(cm->mutex));
    while(cm->map->list_case[testx][testy].element != MAP_VIDE || cm->map->list_case[testx][testy].background == MAP_WATER){
        testx = rand() %40;
        testy = rand() %20;
    }
    // On réserve la place pour le player
    cm->map->list_case[testx][testy].element = MAP_PLAYER;
    pthread_mutex_unlock(&cm->mutex);
    *ret_x = testx;
    *ret_y = testy;
}

void playerMove(completeMap* m, player* p, char mv){    // TODO : Rajouter des "if" pour vérifier si au bord de la map et que la prochaine case est dispo
    pthread_mutex_lock(&m->mutex);    // TODO : Rajouter vérification "if ... != ..."
    switch(mv){
        // TODO : si case avec joueur ou monstre => attaque // artéfact ou trésor => ramasser si possible
        case 'U':
            if (m->map->list_case[p->posY+1][p->posX].background != MAP_WATER && m->map->list_case[p->posY+1][p->posX].element != MAP_PLAYER
            && m->map->list_case[p->posY+1][p->posX].element != MAP_MONSTER && m->map->list_case[p->posY+1][p->posX].element != MAP_OBSTACLE){
                m->map->list_case[p->posY][p->posX].element = MAP_VIDE;
                // TODO : Vérifier si item au sol (interdire si set artefacts rempli ou trop de pièces du grand tout et obtenir item au sol avant de se déplacer)
                m->map->list_case[p->posY+1][p->posX].element = MAP_PLAYER;
                p->posY++;
            }
            break;
        case 'D':
            if (m->map->list_case[p->posY-1][p->posX].background != MAP_WATER && m->map->list_case[p->posY-1][p->posX].element != MAP_PLAYER
            && m->map->list_case[p->posY-1][p->posX].element != MAP_MONSTER && m->map->list_case[p->posY-1][p->posX].element != MAP_OBSTACLE){
                m->map->list_case[p->posY][p->posX].element = MAP_VIDE;
                // TODO : Vérifier si item au sol (interdire si set artefacts rempli ou trop de pièces du grand tout et obtenir item au sol avant de se déplacer)
                m->map->list_case[p->posY-1][p->posX].element = MAP_PLAYER;
                p->posY--;
            }
            break;
        case 'R':
            if (m->map->list_case[p->posY][p->posX+1].background != MAP_WATER && m->map->list_case[p->posY][p->posX+1].element != MAP_PLAYER
            && m->map->list_case[p->posY][p->posX+1].element != MAP_MONSTER && m->map->list_case[p->posY][p->posX+1].element != MAP_OBSTACLE){
                m->map->list_case[p->posY][p->posX].element = MAP_VIDE;
                // TODO : Vérifier si item au sol (interdire si set artefacts rempli ou trop de pièces du grand tout et obtenir item au sol avant de se déplacer)
                m->map->list_case[p->posY][p->posX+1].element = MAP_PLAYER;
                p->posX++;
            }
            break;
        case 'L':
            if (m->map->list_case[p->posY][p->posX-1].background != MAP_WATER && m->map->list_case[p->posY][p->posX-1].element != MAP_PLAYER
            && m->map->list_case[p->posY][p->posX-1].element != MAP_MONSTER && m->map->list_case[p->posY][p->posX-1].element != MAP_OBSTACLE){
                m->map->list_case[p->posY][p->posX].element = MAP_VIDE;
                // TODO : Vérifier si item au sol (interdire si set artefacts rempli ou trop de pièces du grand tout et obtenir item au sol avant de se déplacer)
                m->map->list_case[p->posY][p->posX-1].element = MAP_PLAYER;
                p->posX--;
            }
            break;
        default:
            break;
    }
    pthread_mutex_unlock(&m->mutex);    // TODO : Rajouter vérification "if ... != ..."
}

void monsterMove(completeMap* m, monstre* monster){
    int cas;
    srand(time(NULL));

    while (monster->pv > 0){
        cas = rand() % 4;

        pthread_mutex_lock(&m->mutex);

        switch(cas){
            // TODO : si case avec joueur ou monstre => attaque
            // TODO : faire attention à l'activation des pièces du Grand-Tout
            case 0:     // UP => Il faut que le monstre soit à y = 18 au maximum (sinon il sort du tableau de positions)
                // Case dans la map
                if (monster->posY + 1 < 19){
                    // Case disponible
                    if (m->map->list_case[monster->posY+1][monster->posX].background != MAP_WATER
                    && m->map->list_case[monster->posY+1][monster->posX].element != MAP_OBSTACLE
                    && m->map->list_case[monster->posY+1][monster->posX].element != MAP_TRESOR
                    && m->map->list_case[monster->posY+1][monster->posX].element != MAP_ARTIFACT){
                        // Aucun monstre ou joueur dessus
                        if(m->map->list_case[monster->posY+1][monster->posX].element != MAP_PLAYER
                        && m->map->list_case[monster->posY+1][monster->posX].element != MAP_MONSTER){
                            m->map->list_case[monster->posY][monster->posX].element = MAP_VIDE;
                            m->map->list_case[monster->posY+1][monster->posX].element = MAP_MONSTER;
                            monster->posY++;
                        }/*else{ // Joueur ou monstre dessus
                            // TODO : attaquer() l'entité sur la case
                        }*/
                    }
                }
                break;
            case 1:     // DOWN => Il faut que le monstre soit à y = 1 au minimum (sinon il sort du tableau de positions)
                // Case dans la map
                if (monster->posY - 1 > 0){
                    // Case disponible
                    if(m->map->list_case[monster->posY-1][monster->posX].background != MAP_WATER
                    && m->map->list_case[monster->posY-1][monster->posX].element != MAP_OBSTACLE
                    && m->map->list_case[monster->posY-1][monster->posX].element != MAP_TRESOR
                    && m->map->list_case[monster->posY-1][monster->posX].element != MAP_ARTIFACT){
                        // Aucun monstre ou joueur dessus
                        if (m->map->list_case[monster->posY-1][monster->posX].element != MAP_PLAYER
                        && m->map->list_case[monster->posY-1][monster->posX].element != MAP_MONSTER){
                            m->map->list_case[monster->posY][monster->posX].element = MAP_VIDE;
                            m->map->list_case[monster->posY-1][monster->posX].element = MAP_MONSTER;
                            monster->posY--;
                        }/*else{ // Joueur ou monstre dessus
                            // TODO : attaquer() l'entité sur la case
                        }*/
                    }
                }
                break;
            case 2:     // RIGHT => Il faut que le monstre soit à x = 38 au maximum (sinon il sort du tableau de positions)
                // Case dans la map
                if (monster->posX + 1 < 39){
                    // Case disponible
                    if (m->map->list_case[monster->posY][monster->posX+1].background != MAP_WATER 
                    && m->map->list_case[monster->posY][monster->posX+1].element != MAP_OBSTACLE
                    && m->map->list_case[monster->posY][monster->posX+1].element != MAP_TRESOR
                    && m->map->list_case[monster->posY][monster->posX+1].element != MAP_ARTIFACT){
                        // Aucun monstre ou joueur dessus
                        if (m->map->list_case[monster->posY][monster->posX+1].element != MAP_PLAYER
                        && m->map->list_case[monster->posY][monster->posX+1].element != MAP_MONSTER){
                            m->map->list_case[monster->posY][monster->posX].element = MAP_VIDE;
                            m->map->list_case[monster->posY][monster->posX+1].element = MAP_MONSTER;
                            monster->posX++;
                        }/*else{ // Joueur ou monstre dessus
                            // TODO : attaquer() l'entité sur la case
                        }*/
                    }
                }
                break;
            case 3:     // LEFT => Il faut que le monstre soit à x = 1 au minimum (sinon il sort du tableau de positions)
                // Case dans la map
                if (monster->posX - 1 > 0){
                    // Case disponible
                    if (m->map->list_case[monster->posY][monster->posX-1].background != MAP_WATER 
                    && m->map->list_case[monster->posY][monster->posX-1].element != MAP_OBSTACLE
                    && m->map->list_case[monster->posY][monster->posX-1].element != MAP_TRESOR
                    && m->map->list_case[monster->posY][monster->posX-1].element != MAP_ARTIFACT){
                        // Aucun monstre ou joueur dessus
                        if (m->map->list_case[monster->posY][monster->posX-1].element != MAP_PLAYER
                        && m->map->list_case[monster->posY][monster->posX-1].element != MAP_MONSTER){
                            m->map->list_case[monster->posY][monster->posX].element = MAP_VIDE;
                            m->map->list_case[monster->posY][monster->posX-1].element = MAP_MONSTER;
                            monster->posX--;
                        }/*else{ // Joueur ou monstre dessus
                            // TODO : attaquer() l'entité sur la case
                        }*/
                    }
                }
                break;
            default:
                break;
        }
        pthread_mutex_unlock(&m->mutex);

        // Temps d'attente avant le prochain mouvement
        int sleeping = (int) 5 - 0.05*monster->vitesse_deplacement;
        sleep(sleeping + 1);
    }
}
