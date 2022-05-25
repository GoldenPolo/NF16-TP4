#include "tp4.h"
#define TAILLE_MAX 1000


t_Index * creer_index(void){
    t_Index * index = malloc(sizeof(t_Index));
    index->nb_mots_differents = 0;
    index->nb_mots_total = 0;
    if (index != NULL) {
        return index;
    } else {
        return NULL;
    }
}

t_ListePositions* creer_liste_positions() {
    t_ListePositions *liste;
    liste = malloc(sizeof(t_ListePositions));
    if (liste) {
        return liste;
    }
    else {
        return NULL;
    }
}


int ajouter_position(t_ListePositions *listeP, int ligne, int ordre, int num_phrase) {
    t_Position *nouvel_element = malloc(sizeof(t_Position));
    nouvel_element->numero_ligne = ligne;
    nouvel_element->ordre = ordre;
    nouvel_element->numero_phrase = num_phrase;
    
    
    //si premiere fois
    if (listeP->debut == NULL) {
        nouvel_element->suivant = NULL;
        listeP->debut = nouvel_element;
        listeP->nb_elements = 1;
        return 1;
    } else {

        t_Position *elem_suivant = listeP->debut;
        t_Position *elem_precedent = NULL;

        while (elem_suivant != NULL && elem_suivant->numero_ligne <= ligne && elem_suivant->ordre < ordre) {
            elem_precedent = elem_suivant;
            elem_suivant = elem_suivant->suivant;
        }
        if (elem_suivant != NULL) {
            if (elem_suivant->numero_ligne == ligne && elem_suivant->ordre == ordre) {
            return 0;
            }
        }
        if (elem_precedent != NULL) {
            nouvel_element->suivant = elem_suivant;
            elem_precedent->suivant = nouvel_element;
            listeP->nb_elements += 1;
            return 1;
        }
        else {
            listeP->debut = nouvel_element;
            nouvel_element->suivant = elem_suivant;
            listeP->nb_elements += 1;
            return 1;
        }
    }
}

char * lower_format(char * mot){ //met en minuscule
    for ( ; strlen(mot); ++mot) *mot = tolower(*mot);
    return mot;
}

t_Noeud * rechercher_mot(t_Index * index, char * mot){ //renvoie le noeud ou NULL
    t_Noeud * noeud = index->racine;
    while (noeud != NULL) {
        if (strcmp(mot, noeud->mot) == 0) {
            return noeud;
        }
        else if (strcmp(mot, noeud->mot) > 0) {
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
                if (strcmp(actuel->mot, noeud->mot) < 0) {
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
    int nb_mots_lus = 0;
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
            
            //découpage en mots
            int position_mot_precedent = -1;
            for (int i=0; i<strlen(phrase)+1; i++){
                if (phrase[i] == ' ' || phrase[i] == '\n' || phrase[i] == '\0') {
                    num_ordre++;
                    strcpy(mot, phrase);
                    mot = mot + position_mot_precedent + 1;
                    mot[i-position_mot_precedent] = '\0';
                    
                    //suppression du " " final si il y en a un
                    char * end = mot + strlen(mot) - 1;
                      while(end > mot && isspace(*end))
                        end --;
                      *(end+1) = 0;
                    lower_format(mot);
                    printf("Mot %i : \t %s\n", num_ordre, mot);
                    nb_mots_lus++;
                    
                    //ajout dans l'index
                    t_Noeud * noeud_existant = rechercher_mot(index, mot);
                    if (noeud_existant != NULL) {
                        ajouter_position(&noeud_existant->positions, num_ligne, num_ordre, num_phrase);
                        noeud_existant->nb_occurences++;
                    } else {
                        t_Noeud * noeud = malloc(sizeof(t_Noeud));
                        noeud->mot = malloc(sizeof(mot));
                        strcpy(noeud->mot, mot);
                        noeud->filsDroit = NULL;
                        noeud->filsGauche = NULL;
                        t_ListePositions * liste_positions = creer_liste_positions();
                        ajouter_position(liste_positions, num_ligne, num_ordre, num_phrase);
                        noeud->positions = *liste_positions; //correct ?
                        noeud->nb_occurences = 1;
                        ajouter_noeud(index, noeud);
                        index->nb_mots_differents++;
                    }
                    index->nb_mots_total++;
                    // a tester
                    
                    position_mot_precedent = i;
                }
            }
            ptr_l = strtok(NULL, delim_l);
        }
    }
    free(ligne);
    fclose(fichier);
    return nb_mots_lus;
}

