#ifndef TP4_TP4_H
#define TP4_TP4_H
#endif //TP4_TP4_H
#define TAILLE_MAX 1000

typedef struct t_position {
    int numero_ligne;
    int ordre;
    int numero_phrase;
    int placement_phrase;
    struct t_position * suivant;
} t_Position;

typedef struct t_listePositions {
    t_Position * debut;
    int nb_elements;
} t_ListePositions;

typedef struct t_noeud {
    char * mot;
    int nb_occurences;
    t_ListePositions positions;
    struct t_noeud *filsGauche;
    struct t_noeud *filsDroit;
} t_Noeud;

typedef struct t_index {
    t_Noeud * racine;
    int nb_mots_differents;
    int nb_mots_total;
} t_Index;

t_ListePositions * creer_liste_positions();

int ajouter_position(t_ListePositions * listeP, int ligne, int ordre, int num_phrase, int placement_phrase);

t_Index * creer_index();

t_Noeud * rechercher_mot(t_Index * index, char * mot);

int ajouter_noeud(t_Index * index, t_Noeud * noeud);

int indexer_fichier(t_Index * index, char * filename);

void afficher_index(t_Index * index);

void afficher_max_apparition(t_Index * index);

void afficher_occurrences_mot(t_Index * index, char * mot);

void construire_texte(t_Index * index, char * filename);

// rajouté
int parcours_infixe(t_Noeud * noeud, t_Noeud * tableau[], int i);
