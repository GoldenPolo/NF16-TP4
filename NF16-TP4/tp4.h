
typedef struct t_position {
    int numero_ligne;
    int ordre;
    int numero_phrase;
    struct t_position * suivant;
} t_position;

typedef struct t_listePositions {
    t_position * debut;
    int nb_elements;
} t_listePositions;

typedef struct t_noeud {
    char * mot;
    int nb_occurences;
    t_listePositions positions;
    struct t_noeud * filsGauche;
    struct t_noeud * filsDroit;
} t_noeud;

typedef struct t_index {
    t_noeud * racine;
    int nb_mots_differents;
    int nb_mots_total;
} t_index;

t_listePositions * creer_liste_positions();

int ajouter_position(t_listePositions * listeP, int ligne, int ordre, int num_phrase);

t_index * creer_index();

t_noeud * rechercher_mot(t_index * index, char * mot);

int ajouter_noeud(t_index * index, t_noeud * noeud);

//test
