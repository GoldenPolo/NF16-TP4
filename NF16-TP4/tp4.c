#include "tp4.h"
#define TAILLE_MAX 1000

t_Index * creer_index(void){
    t_Index * index = malloc(sizeof(t_Index));
    if (index != NULL) {
        return index;
    } else {
        return NULL;
    }
}

char * lower_format(char * mot){ //met en minuscule
    for ( ; sizeof(mot); ++mot) *mot = tolower(*mot);
}

t_Noeud * rechercher_mot(t_Index * index, char * mot){ //renvoie le mot ou NULL
    mot = lower_format(mot);
    t_Noeud * noeud = index->racine;
    while (noeud != NULL) {
        if (noeud->mot == mot) {
            return noeud;
        }
        else if (noeud->mot < mot) {
            noeud = noeud->filsDroit;
        }
        else {
            noeud = noeud->filsGauche;
        }
    }
    return NULL;
}


int ajouter_noeud(t_Index * index, t_Noeud * noeud){
    if (rechercher_mot(index, noeud->mot) != NULL) {
        // noeud existe déjà
        return 0;
    }
    else {
        //si noeud a rajouter
        if (index->racine == NULL) {
            index->racine = noeud;
        }
        else {
            t_Noeud * actuel = index->racine;
            t_Noeud * pere = NULL;
            int derniere_action;
            while (actuel != NULL) {
                if (actuel->mot < noeud->mot) {
                    pere = actuel;
                    actuel = actuel->filsDroit;
                    derniere_action = 1;
                }
                else {
                    pere = actuel;
                    actuel = actuel->filsGauche;
                    derniere_action = 0;
                }
            }
            if (derniere_action == 1) {
                pere->filsDroit = noeud;
            }
            else {
                pere->filsGauche = noeud;
            }
        }
        return 1;
    }
}

int indexer_fichier(t_Index * index, char * filename){
    FILE * fichier = fopen(filename, "r");
    if (fichier == NULL)
    {
        printf("Le fichier est introuvable\n");
        return EXIT_FAILURE;
    }
    int num_ligne = 0;
    int num_ordre = 0;
    int num_phrase = 0;
    char * ligne = malloc(sizeof(sizeof(char)*TAILLE_MAX));
    // decoupage en lignes
    while (fgets(ligne, TAILLE_MAX, fichier) != NULL) {
        num_ligne ++;
        printf("Ligne %i :\t %s", num_ligne, ligne);
        num_ordre = 0;
        //découpage en phrases
        char delim_l[] = ".\n";
        char *ptr_l = strtok(ligne, delim_l);
        char * phrase= malloc(sizeof(sizeof(char)*TAILLE_MAX));
        char * mot= malloc(sizeof(phrase));
        while(ptr_l != NULL){
            num_phrase ++;
            phrase = ptr_l;
            //suppression du ' ' initial si il y en a un
            if (phrase[0] == ' ') phrase++;
            printf("Phrase %i : \t %s\n", num_phrase, phrase);
            //suppression du ' ' initial si il y en a un
            if (phrase[0] == ' ') {
                phrase++;
            }
            //découpage en mots
            int position_mot_precedent = -1;
            for (int i=0; i<strlen(phrase)+1; i++){
                if (phrase[i] == ' ' || phrase[i] == '\n' || phrase[i] == '\0') {
                    num_ordre++;
                    strcpy(mot, phrase);
                    mot = mot + position_mot_precedent + 1;
                    mot[i-position_mot_precedent] = '\0';
                    mot = lower_format(mot);
                    printf("Mot %i : \t %s\n", num_ordre, mot);
                    
                    //ajout dans l'index
                    if (rechercher_mot(index, mot) != NULL) {
                        ajouter_position(&rechercher_mot(index, mot)->positions, num_ligne, num_ordre, num_phrase);
                    } else {
                        t_Noeud * noeud = malloc(sizeof(t_Noeud));
                        noeud->mot = mot;
                        noeud->filsDroit = NULL;
                        noeud->filsGauche = NULL;
                        t_ListePositions * liste_positions = creer_liste_positions();
                        ajouter_position(liste_positions, num_ligne, num_ordre, num_phrase);
                        noeud->positions = *liste_positions; //correct ?
                        noeud->nb_occurences = 1;
                        ajouter_noeud(index, noeud);
                    }
                    // a tester
                    
                    position_mot_precedent = i;
                }
            }
            ptr_l = strtok(NULL, delim_l);
        }
    }
    free(ligne);
    fclose(fichier);
    return 1;
}
