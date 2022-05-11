#include "tp4.h"


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
    if (existe_noeud(index->racine, noeud->mot) == 1) {
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
