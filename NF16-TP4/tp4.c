#include "tp4.h"


char * lower_format(char * mot){
    for ( ; sizeof(mot); ++mot) *mot = tolower(*mot);
}

int existe_noeud(t_Noeud * noeud, char * mot){
    mot = lower_format(mot);
    if (noeud == NULL) {
        return 0;
    }
    else if (noeud->mot == mot) {
        return 1;
    }
    else if (noeud->mot < mot){
        return existe_noeud(noeud->filsGauche, mot);
    }
    else {
        return existe_noeud(noeud->filsDroit, mot);
    }
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
        
        
    }
}
