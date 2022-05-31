#include "tp4.h"
#define TAILLE_MAX 1000


t_Index * creer_index(void){
    t_Index * index = malloc(sizeof(t_Index));
    index->nb_mots_differents = 0;
    index->nb_mots_total = 0;
    if (index != NULL) {
        return index;
    } else {
        free(index);
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
        free(liste);
        return NULL;
    }
}


int ajouter_position(t_ListePositions *listeP, int ligne, int ordre, int num_phrase, int placement_phrase) {
    t_Position *nouvel_element = malloc(sizeof(t_Position));
    nouvel_element->numero_ligne = ligne;
    nouvel_element->ordre = ordre;
    nouvel_element->numero_phrase = num_phrase;
    nouvel_element->placement_phrase = placement_phrase;
    
    
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
    int compare;
    while (noeud != NULL) {
        compare = strcmp(mot, noeud->mot);
        if (compare == 0) {
            return noeud;
        }
        else if (compare > 0) {
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
        //printf("Ligne %i :\t %s", num_ligne, ligne);
        num_ordre = 0;
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
            //printf("Phrase %i : \t %s\n", num_phrase, phrase);
            
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
                    //printf("Mot %i : \t %s\n", num_ordre, mot);
                    nb_mots_lus++;
                    
                    //ajout dans l'index
                    t_Noeud * noeud_existant = rechercher_mot(index, mot);
                    if (noeud_existant != NULL) {
                        ajouter_position(&noeud_existant->positions, num_ligne, num_ordre, num_phrase, i);
                        noeud_existant->nb_occurences++;
                    } else {
                        t_Noeud * noeud = malloc(sizeof(t_Noeud));
                        noeud->mot = malloc(sizeof(mot));
                        strcpy(noeud->mot, mot);
                        noeud->filsDroit = NULL;
                        noeud->filsGauche = NULL;
                        t_ListePositions * liste_positions = creer_liste_positions();
                        ajouter_position(liste_positions, num_ligne, num_ordre, num_phrase, i);
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
    printf("Fichier correctement indexe !\n");
    return nb_mots_lus;
}

void afficher_index(t_Index * index){
    t_Noeud * tableau[index->nb_mots_differents];
    t_Noeud * noeud = malloc(sizeof(t_Noeud));
    noeud = index->racine;
    parcours_infixe(noeud, tableau, 0);
    //affichage du tableau
    char lettre_precedente = '*';
    for (int i = 0; i<index->nb_mots_differents; i++) {
        //nouvelle lettre ?
        if (i == 0 || lettre_precedente != tableau[i]->mot[0]) {
            lettre_precedente = tableau[i]->mot[0];
            printf("|\n\n%c\n", lettre_precedente-32);
        }
        //affichage du mot
        tableau[i]->mot[0] = tableau[i]->mot[0]-32;
        printf("| - - %s\n", tableau[i]->mot);
        //affichage de ses positions
        t_Position * position = tableau[i]->positions.debut;
        while (position != NULL) {
            printf("| - - - (l:%i, o:%i, p:%i)\n", position->numero_ligne, position->ordre, position->numero_phrase);
            position = position->suivant;
        }
    }
    free(noeud);
}

int parcours_infixe(t_Noeud * noeud, t_Noeud * tableau[], int i){
    // remplit le tableau avec les noeuds triés par ordre alphabetique
    if (noeud != NULL) {
        i = parcours_infixe(noeud->filsGauche, tableau, i);
        //afficher(n)
        tableau[i] = noeud;
        i = i+1;
        i = parcours_infixe(noeud->filsDroit, tableau, i);
    }
    
    return i;
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

    printf("\n\nLe mot %s apparait le plus dans le texte : %i fois\n", le_mot, compteur);
}

void afficher_occurences_mot(t_Index * index, char * mot){
    return;
}

void construire_texte(t_Index * index, char * filename){
    t_Texte_liste * texte = malloc(sizeof(t_Texte_liste));
    texte->nombre_phrases = 0;
    //creation d'un tableau comme pour la 7
    t_Noeud * tableau_t[index->nb_mots_differents];
    t_Noeud * noeud = malloc(sizeof(t_Noeud));
    noeud = index->racine;
    parcours_infixe(noeud, tableau_t, 0);
    // pour chaque mot de l'index
    t_Position * position = malloc(sizeof(t_Position));
    for (int i = 0; i<index->nb_mots_differents; i++) {
        // pour chaque position de ce mot
        position = tableau_t[i]->positions.debut;
        for (int j = 0; j<tableau_t[i]->positions.nb_elements; j++) {
            //rajoute tous les mots sans les trier
            
            rajouter_mot_maillon(texte, tableau_t[i]->mot, position);
            position = position->suivant;
        }
    }
    tri_texte(texte);
    free(position);
    //free(noeud);
    
}

void rajouter_mot_maillon(t_Texte_liste * texte, char * mot, t_Position * position){
    t_Phrase_liste * phrase = texte->debut;
    t_Phrase_liste * phrase_precedente = NULL;
    
    
    while (phrase->suivant != NULL && phrase->numero_phrase != position->numero_phrase) {
        phrase_precedente = phrase;
        phrase = phrase->suivant;
    }
    
    //si premiere fois de la phrase
    if (phrase == NULL) {
        t_Phrase_liste * phrase = malloc(sizeof(t_Phrase_liste));
        phrase->ligne = position->numero_ligne;
        phrase->nombre_mots = 1;
        phrase->numero_phrase = position->numero_phrase;
        phrase->suivant = NULL;
        t_Mot_maillon * mot_maillon = malloc(sizeof(t_Mot_maillon));
        mot_maillon->mot = mot;
        mot_maillon->ordre = position->ordre;
        mot_maillon->suivant = NULL;
        phrase->debut = mot_maillon;
        if (phrase_precedente == NULL) {
            texte->debut = phrase;
        } else {
            phrase_precedente->suivant = phrase;
        }
        texte->nombre_phrases++;
    } else {
        //sinon
        
        t_Mot_maillon * mot_maillon = malloc(sizeof(t_Mot_maillon));
        mot_maillon = phrase->debut;
        while (mot_maillon->suivant != NULL) {
            mot_maillon = mot_maillon->suivant;
        }
        t_Mot_maillon * nouveau_mot_maillon = malloc(sizeof(t_Mot_maillon));
        nouveau_mot_maillon->mot = mot;
        nouveau_mot_maillon->ordre = position->ordre;
        nouveau_mot_maillon->suivant = NULL;
        mot_maillon->suivant = nouveau_mot_maillon;
    }
}

void tri_texte(t_Texte_liste * texte){
    //tri par lignes
    if (texte->debut->suivant == NULL) {
        //texte d'un mot
        return;
    }
    t_Phrase_liste * phrase_precedente = NULL;
    t_Phrase_liste * phrase_actuelle = texte->debut;
    t_Phrase_liste * phrase_suivante = texte->debut->suivant;
    for (int i =0; i<texte->nombre_phrases; i++) {
        
        for (int j=0; j<texte->nombre_phrases; j++) {
            while (phrase_suivante != NULL) {
                
                if (phrase_actuelle->ligne > phrase_suivante->ligne) {
                    if (phrase_precedente != NULL) {
                        phrase_precedente->suivant = phrase_suivante;
                    }
                    phrase_actuelle->suivant = phrase_suivante->suivant;
                    
                    phrase_suivante->suivant = phrase_actuelle;
                    //passage aux suivants spécial
                    
                    phrase_precedente = phrase_suivante;
                    
                    phrase_suivante = phrase_actuelle->suivant;
                    
                } else {
                //passage normal
                phrase_precedente = phrase_actuelle;
                phrase_actuelle = phrase_suivante;
                phrase_suivante = phrase_suivante->suivant;
                }
            }
        }
    }
    
    //tri par phrases
    
    
    //tri par mots
}
