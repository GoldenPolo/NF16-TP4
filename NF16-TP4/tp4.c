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

