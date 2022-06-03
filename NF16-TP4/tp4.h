#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
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
    struct t_noeud * filsGauche;
    struct t_noeud * filsDroit;
} t_Noeud;

typedef struct t_index {
    t_Noeud * racine;
    int nb_mots_differents;
    int nb_mots_total;
} t_Index;

t_ListePositions * creer_liste_positions(void);

int ajouter_position(t_ListePositions * listeP, int ligne, int ordre, int num_phrase, int placement_phrase);

t_Index * creer_index(void);

t_Noeud * rechercher_mot(t_Index * index, char * mot);

int ajouter_noeud(t_Index * index, t_Noeud * noeud);

int indexer_fichier(t_Index * index, char * filename);

void afficher_index(t_Index * index);

void afficher_max_apparition(t_Index * index);

void afficher_occurences_mot(t_Index * index, char * mot);

void construire_texte(t_Index * index, char * filename);

//ajouts

typedef struct t_mot_maillon {
    char * mot;
    int ordre;
    struct t_mot_maillon * suivant;
} t_Mot_maillon;

typedef struct t_phrase_liste {
    t_Mot_maillon * debut;
    int ligne;
    int nombre_mots;
    int numero_phrase;
    struct t_phrase_liste * suivant;
} t_Phrase_liste;

typedef struct t_texte_liste {
    t_Phrase_liste * debut;
    int nombre_phrases;
} t_Texte_liste;

char * lower_format(char * mot);

int parcours_infixe(t_Noeud * noeud, t_Noeud * tableau[], int i);

int maximum(int a, int b);

int hauteur(t_Noeud *noeud);

typedef struct t_pile {
    t_Noeud * tableau[TAILLE_MAX];
    int sommet;
} t_Pile;

typedef struct t_MotPhrase {
    char * mot;
    int ordre;
    struct t_MotPhrase * suivant;
} t_MotPhrase;

typedef struct t_Phrase {
    t_MotPhrase * debut;
    struct t_Phrase * suivant;
    int nb_elements;
} t_Phrase;

t_Pile * creer_pile(void);

int pile_vide(t_Pile * pile);

int pile_pleine(t_Pile * pile);

int empiler(t_Pile * pile, t_Noeud * noeud);

t_Noeud * depiler(t_Pile * pile);

t_MotPhrase * creer_mot_phrase(void);

t_Phrase * creer_phrase(void);

void ajout_mot_dans_phrase(t_Phrase * phrase, int ordre, char * mot);

void afficher_phrase(t_Phrase * phrase);
