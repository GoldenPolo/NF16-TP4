#include "tp4.h"
#define TAILLE_MAX 1000



t_Index * creer_index(void){
    //cree un index vide et le renvoie
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
    //cree une liste de positions vide et la renvoie
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
    //ajoute un element a la liste fournie en parametre
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

char * lower_format(char * mot){
    //met en minuscule
    for ( ; strlen(mot); ++mot) *mot = tolower(*mot);
    return mot;
}

t_Noeud * rechercher_mot(t_Index * index, char * mot){
    //renvoie le noeud ou NULL
    t_Noeud * noeud = malloc(sizeof(t_Noeud));
    noeud = index->racine;
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
    free(noeud);
    return NULL;
}


int ajouter_noeud(t_Index * index, t_Noeud * noeud){
    //ajoute le noeud passe en parametre a l'index
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
    //cree un index a partir d'un fichier texte
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

void afficher_index(t_Index * index){
    //affiche tous les mots contenus dans l'index
    t_Noeud * tableau[index->nb_mots_differents];
    t_Noeud * noeud = malloc(sizeof(t_Noeud));
    noeud = index->racine;
    parcours_infixe(noeud, tableau, 0);
    //affichage du tableau
    char lettre_precedente = '*';
    for (int i = 0; i<index->nb_mots_differents; i++){
        //nouvelle lettre ?
        if (i == 0 || lettre_precedente != tableau[i]->mot[0]){
            lettre_precedente = tableau[i]->mot[0];
            printf("|\n\n%c\n", lettre_precedente-32);
        }
        //affichage du mot
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

void afficher_max_apparition(t_Index * index) {
    //affiche le mot avec le plus d'apparitions et ses occurences
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
    printf("\n\nLe mot \"%s\" apparait le plus dans le texte : %i fois\n", le_mot, compteur);
}

int maximum(int a, int b){
    //renvoie le max entre 2 entiers
    if (a<b) return b;
    else return a;
}

int hauteur(t_Noeud *noeud){
    //calcul recursif de la hauteur d'un noeud
    if (noeud == NULL) return 0;
    else {
        return 1 + maximum(hauteur(noeud->filsGauche), hauteur(noeud->filsDroit));
    }
}

//2 dernieres questions - CES FONCTIONS ONT ETE CREES SUR UNE AUTRE MACHINE D'OU LES LEGERS CHANGEMENTS

//rajout des structures de pile et de phrases

t_Pile * creer_pile(void){
    //cree une pile vide et la renvoie
    t_Pile * pile = malloc(sizeof(t_Pile));
    pile->sommet = -1;
    return pile;
}

int pile_vide(t_Pile * pile){
    //test si pile est vide
    return (pile->sommet == -1);
}

int pile_pleine(t_Pile * pile){
    //test si pile est pleine
    return (pile->sommet == TAILLE_MAX - 1);
}

int empiler(t_Pile * pile, t_Noeud * noeud){
    //rajoute un element a la pile
    if (pile_pleine(pile)) return -1;
    else {
        pile->tableau[pile->sommet] = noeud;
        pile->sommet++;
        return pile->sommet;
    }
}

t_Noeud * depiler(t_Pile * pile){
    //depile l'element au sommet de la pile
    if (pile_vide(pile)) return NULL;
    else {
        t_Noeud * noeud = malloc(sizeof(t_Noeud));
        noeud = pile->tableau[pile->sommet];
        pile->sommet=pile->sommet-1;
        return noeud;
    }
}

t_Phrase * creer_phrase(void){
    //cree une phrase vide et l'initialise
    t_Phrase * phrase = malloc(sizeof(t_Phrase));
    phrase->debut = NULL;
    phrase->nb_elements = 0;
    phrase->suivant=NULL;
    return phrase;
}

void ajout_mot_dans_phrase(t_Phrase * phrase, int ordre, char * mot){
    //ajoute le mot dans la phrase
    t_MotPhrase * parcourir = phrase->debut;
    t_MotPhrase * precedent = NULL;
    t_MotPhrase * nouveau_mot = malloc(sizeof(t_MotPhrase));
    nouveau_mot->ordre = ordre;
    nouveau_mot->mot = mot;

    while (parcourir != NULL && parcourir->ordre < ordre){ //recherche
        precedent = parcourir;
        parcourir = parcourir->suivant;
    }
    if (precedent==NULL){ //insertion
        nouveau_mot->suivant = phrase->debut;
        phrase->debut = nouveau_mot;
    } else {
        nouveau_mot->suivant = parcourir;
        precedent->suivant=nouveau_mot;
    }
    phrase->nb_elements ++;
}

void afficher_phrase(t_Phrase * phrase){
    //affiche la phrase entiere
    t_MotPhrase * mot_de_parcours = malloc(sizeof(t_MotPhrase));
    mot_de_parcours = phrase->debut;
    while(mot_de_parcours != NULL){
        printf(" %s", mot_de_parcours->mot);
        mot_de_parcours = mot_de_parcours->suivant;
    }
    free(mot_de_parcours);
}

void afficher_occurences_mot(t_Index * index, char * mot){
    //affiche les occurences et la phrase correspondante
    t_Noeud * noeud_de_parcours = malloc(sizeof(t_Noeud));
    t_Noeud * noeud_cherche = malloc(sizeof(t_Noeud));
    noeud_cherche = rechercher_mot(index,mot);
    t_Position * positions_du_noeud = malloc(sizeof(t_Position));
    positions_du_noeud->suivant = NULL;
    positions_du_noeud->numero_phrase = 0;
    positions_du_noeud->placement_phrase = 0;
    positions_du_noeud->ordre = 0;
    positions_du_noeud->numero_ligne = 0;
    t_Pile * pile_noeud = creer_pile();

    if (noeud_cherche != NULL) {
        printf("Occurences : %i\n", noeud_cherche->positions.nb_elements);
        t_Position * positions_mot_ref = malloc(sizeof(t_Position));
        positions_mot_ref = noeud_cherche->positions.debut;

        while (positions_mot_ref != NULL) { //chaque position est comparee
            noeud_de_parcours = index->racine;
            t_Phrase * phrase = creer_phrase();
            
            while (noeud_de_parcours != NULL || pile_vide(pile_noeud) != 1){ //parcours de l'arbre
                while (noeud_de_parcours != NULL){
                    empiler(pile_noeud, noeud_de_parcours);
                    noeud_de_parcours = noeud_de_parcours->filsGauche;
                }
                noeud_de_parcours = depiler(pile_noeud);
                positions_du_noeud = noeud_de_parcours->positions.debut;

                while (positions_du_noeud != NULL){ //parcours des positions
                    if (positions_du_noeud->numero_phrase == positions_mot_ref->numero_phrase){
                        ajout_mot_dans_phrase(phrase, positions_du_noeud->ordre, noeud_de_parcours->mot);
                    }
                    positions_du_noeud=positions_du_noeud->suivant;
                }
                noeud_de_parcours=noeud_de_parcours->filsDroit;
            }
            
            printf("| Ligne %d, mot %d : ", positions_mot_ref->numero_ligne, positions_mot_ref->ordre);
            afficher_phrase(phrase);
            printf("\n");
            positions_mot_ref = positions_mot_ref->suivant;
            free(phrase);
        }
    } else {
        printf("Mot introuvable.\n");
    }
    free(noeud_de_parcours);
    free(noeud_cherche);
    free(noeud_de_parcours);
}

void construire_texte(t_Index * index, char * filename){
    //reconstruit un texte a partir de l'index
    FILE * file = fopen(filename, "w");
    
    int nb_phrase = 1;
    int taille_phrase = 0;
    
    t_Noeud * noeud_de_parcours = malloc(sizeof(t_Noeud));
    t_Position * positions_du_noeud = malloc(sizeof(t_Noeud));
    positions_du_noeud->suivant = NULL;
    positions_du_noeud->numero_phrase = 0;
    positions_du_noeud->placement_phrase = 0;
    positions_du_noeud->ordre = 0;
    positions_du_noeud->numero_ligne = 0;
    t_Pile * pile_noeud = creer_pile();
    t_Phrase * phrase = creer_phrase();
    
    while (taille_phrase != 0 || nb_phrase == 1){ //chaque position est comparee
        noeud_de_parcours = index->racine;
        phrase = creer_phrase();
        while (noeud_de_parcours != NULL || pile_vide(pile_noeud) != 1){ //parcours de l'arbre
            while (noeud_de_parcours != NULL){
                empiler(pile_noeud, noeud_de_parcours);
                noeud_de_parcours = noeud_de_parcours->filsGauche;
            }
            noeud_de_parcours = depiler(pile_noeud);
            positions_du_noeud = noeud_de_parcours->positions.debut;
            while(positions_du_noeud != NULL){ //parcours des positions
                if(positions_du_noeud->numero_phrase == nb_phrase){
                    ajout_mot_dans_phrase(phrase, positions_du_noeud->ordre, noeud_de_parcours->mot);
                }
                positions_du_noeud = positions_du_noeud->suivant;
            }
            noeud_de_parcours = noeud_de_parcours->filsDroit;
        }
        fprintf(file, "\n");
        nb_phrase++;
        taille_phrase = phrase->nb_elements;
        free(phrase);
        //ecriture dans le fichier
        t_MotPhrase * mot = phrase->debut;
        if (taille_phrase != 0){
            while(mot != NULL){
                if (phrase->debut == mot){
                    mot->mot[0] = mot->mot[0]-32;
                    fprintf(file,"%s", mot->mot);
                }
                else if (mot->suivant == NULL){
                    fprintf(file," %s.", mot->mot);
                } else {
                    fprintf(file," %s", mot->mot);
                }
                mot = mot->suivant;
            }
        }
    }
    fclose(file);
    free(noeud_de_parcours);
    free(positions_du_noeud);
    printf("Fichier cree !.\n");
}
