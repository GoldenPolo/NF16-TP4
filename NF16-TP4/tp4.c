#include "tp4.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "ctype.h"

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


int ajouter_position(t_ListePositions *listeP, int ligne, int ordre, int num_phrase, int placement_phrase) {
    t_Position *nouvel_element = malloc(sizeof(t_Position));
    nouvel_element->numero_ligne = ligne;
    nouvel_element->ordre = ordre;
    nouvel_element->numero_phrase = num_phrase;
    nouvel_element->placement_phrase = placement_phrase;

    t_Position *elem_suivant = listeP->debut;
    t_Position *elem_precedent = NULL;

    while (elem_suivant != NULL && elem_suivant->numero_ligne <= ligne && elem_suivant->ordre < ordre) {
        elem_precedent = elem_suivant;
        elem_suivant = elem_suivant->suivant;
    }
    if (elem_suivant->numero_ligne == ligne && elem_suivant->ordre == ordre) {
        return 0;
    }
    else {
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

t_Index * creer_index() {
    t_Index *index = malloc(sizeof(t_Index));
    index->nb_mots_differents = 0;
    index->nb_mots_total = 0;
    if (index != NULL) {
        return index;
    }
    else {
        return NULL;
    }
}

t_Noeud * rechercher_mot(t_Index * index, char * mot){
    strlwr(mot);
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
        printf("Ligne %i : %s", num_ligne, ligne);

        //découpage en phrases
        char delim_l[] = ".\n";
        char *ptr_l = strtok(ligne, delim_l);
        char * phrase = malloc(sizeof(sizeof(char)*TAILLE_MAX));
        char * mot = malloc(sizeof(phrase));
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
                    strlwr(mot);
                    printf("Mot %i : \t %s\n", num_ordre, mot);
                    nb_mots_lus++;

                    //ajout dans l'index
                    t_Noeud * noeud_existant = rechercher_mot(index, mot);
                    if (noeud_existant != NULL) {
                        ajouter_position(&(noeud_existant->positions), num_ligne, num_ordre, num_phrase, i);
                        noeud_existant->nb_occurences++;
                        printf("Ajoute a l'index");
                    }
                    else {
                        t_Noeud * noeud = malloc(sizeof(t_Noeud));
                        noeud->mot = malloc(sizeof(mot));
                        noeud->filsDroit = NULL;
                        noeud->filsGauche = NULL;
                        t_ListePositions * liste_positions = creer_liste_positions();
                        ajouter_position(liste_positions, num_ligne, num_ordre, num_phrase, i);
                        noeud->positions = *liste_positions; //correct ?
                        noeud->nb_occurences = 1;
                        ajouter_noeud(index, noeud);
                        index->nb_mots_differents++;
                        printf("Ajoute a l'index");
                    }
                    index->nb_mots_total++;
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

int parcours_infixe(t_Noeud * noeud, t_Noeud * tableau[], int i) {
    // remplit le tableau avec les noeuds tries par ordre alphabétique
    if (noeud != NULL) {
        i = parcours_infixe(noeud->filsGauche, tableau, i);
        tableau[i] = noeud;
        i = i + 1;
        i = parcours_infixe(noeud->filsDroit, tableau, i);
    }
    return i;
}

void afficher_index(t_Index * index) {
    t_Noeud *tableau[index->nb_mots_differents];
    t_Noeud *noeud = index->racine;
    parcours_infixe(noeud, tableau, 0);
    char lettre_precedente = '*';
    for (int i=0; i<index->nb_mots_differents; i++) {
        // test si premiere lettre du mot actuel est differente de celle du mot precedent
        if (i == 0 || tableau[i]->mot[0] != lettre_precedente) {
            lettre_precedente = tableau[i]->mot[0];
            printf("|\n\n%c\n", lettre_precedente-32);
        }
        // affichage du mot
        tableau[i]->mot[0] = (tableau[i]->mot[0])-32;
        printf("|-- %s\n", tableau[i]->mot);
        // affichage des positions
        t_ListePositions positions = tableau[i]->positions;
        t_Position *position = positions.debut;
        for (int j=0; j<positions.nb_elements; j++) {
            printf("|---- (l:%i, o:%i, p:%i)\n", position->numero_ligne, position->ordre, position->numero_phrase);
            position = position->suivant;
        }
    }
}

void afficher_max_apparition(t_Index * index) {
    t_Noeud *tableau[index->nb_mots_differents];
    t_Noeud *noeud = index->racine;
    parcours_infixe(noeud, tableau, 0);
    int nb_occurences;
    int compteur = 0;
    char *le_mot;

    for (int i=0; i<index->nb_mots_differents; i++) {
        nb_occurences = tableau[i]->positions.nb_elements;
        if (nb_occurences > compteur) {
            compteur = nb_occurences;
            le_mot = tableau[i]->mot;
        }
    }

    printf("Le mot %s apparait le plus dans le texte : %i fois", le_mot, compteur);
}

void afficher_occurences_mot(t_Index * index, char * mot) {
    strlwr(mot);
    t_Noeud *noeud_entree = rechercher_mot(index, mot);
    if (noeud_entree == NULL) {
        printf("Le mot n'existe pas dans le fichier !");
        return;
    }
    mot[0] = mot[0]-32;
    printf("Mot = \"%s\"\n", mot);
    printf("Occurences = %i\n", noeud_entree->positions.nb_elements);
    t_Position *occurrence = noeud_entree->positions.debut;

    // récupération de tous les mots de l'index
    t_Noeud *noeuds_index[index->nb_mots_differents];
    parcours_infixe(index->racine, noeuds_index, 0);

    // Construction et affichage de chaque phrase possédant le mot entré
    t_Noeud *phrase[50];
    for (int i=0; i<noeud_entree->positions.nb_elements; i++) {
        int taille_phrase = 0;
        // Parcours de chaque noeud de l'index
        for (int j=0; j<index->nb_mots_differents; j++) {
            t_Position * position_elem_teste = noeuds_index[j]->positions.debut;
            // Parcours de chaque occurrence du mot correspondant au noeud
            for (int l=0; l<noeuds_index[j]->positions.nb_elements; l++) {
                // Test si le numero de phrase d'une des occurrences correspond
                // à celui du mot entré
                if (position_elem_teste->numero_phrase == occurrence->numero_phrase) {
                    // Ajout du noeud à la phrase à la position du mot dans celle ci
                    phrase[position_elem_teste->placement_phrase] = noeuds_index[j];
                    taille_phrase ++;
                }
                position_elem_teste = position_elem_teste->suivant;
            }
        }
        // Affichage de la position du mot entré et de la phrase
        printf("| Ligne %i, mot %i :", occurrence->numero_ligne, occurrence->ordre);
        for (int p=0; p<taille_phrase; p++) { // < ou <= ?? Cas ou la phrase ne contient qu'un mot ?
            printf(" %s", phrase[p]->mot);
        }
        printf(".\n");
        occurrence = occurrence->suivant;
    }
}
