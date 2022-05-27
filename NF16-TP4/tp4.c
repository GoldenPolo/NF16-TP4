#include "tp4.h"
#include "stdio.h"
#include "stdlib.h"

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
    t_Index *index;
    index = malloc(sizeof(t_Index));
    if (index) {
        return index;
    }
    else {
        return NULL;
    }
}

void afficher_index(t_Index *index, char *mot_precedent) {
    if (index != NULL) {
        t_Noeud *x = index->racine;
        afficher_index(*(x->filsGauche), x->mot);
        char *mot = x->mot;
        char lettre = mot[0];
        if (mot_precedent[0] != lettre) {
            strupr(&lettre);
            printf("|\n\n%c\n", lettre);
        }
        char motMaj = lettre;
        for (int i=1; i< strlen(mot); i++) {
            motMaj += mot[i];
        }
        printf("|-- %s\n", motMaj);
        t_ListePositions positions = x->positions;
        t_Position *position = positions.debut;
        for (int i=0; i < positions.nb_elements; i++) {
            printf("|---- (l:%i, o:%i, p:%i)\n", position->numero_ligne, position->ordre, position->numero_phrase);
            position = position->suivant;
        }
        afficher_index(*(x->filsDroit), x->mot);
    }
}
