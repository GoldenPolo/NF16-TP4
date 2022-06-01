#include "tp4.h"

void main(){
    int test = 1;
    t_Index *index = creer_index();
    int a_indexe = 0;
    
    while (test == 1) {
        printf("\n\nMENU\n-----\n1 = charger un fichier\n2 = caracteristiques de l'index\n3 = afficher index\n"
               "4 = rechercher un mot\n5 = afficher le mot avec le maximum d'apparitions\n"
               "6 = afficher les occurrences d'un mot\n7 = construire le texte a partir de l'index\n8 = quitter\n");
        printf("Votre choix : ");
        int choix;
        scanf("%i", &choix);

        switch (choix) {
            case 1:
                //printf("\nNom du fichier:");
                //char nom_fichier;
                //scanf("%s", &nom_fichier);
                // "C:\\Users\\alexa\\Documents\\UTC\\TC03 - P22\\NF16\\TP\\TP4\\TP4\\";
                indexer_fichier(index, "/Users/pol/Desktop/UTC/TC04/NF16/Algos/NF16/NF16-TP4/NF16-TP4/test.txt");
                a_indexe = 1;
                printf("Nombre de mots indexes : %i\nDont %i mots differents", index->nb_mots_total, index->nb_mots_differents);
                break;
            case 2:
                if (a_indexe == 0) {
                    printf("\nAucun fichier n'est indexe.\n");
                    break;
                }
                printf("Nombre de mots dans l'index : %i\nDont %i mots differents\n", index->nb_mots_total, index->nb_mots_differents);
                //faire equilibre !!!!
                int delta = (hauteur(index->racine->filsDroit) - hauteur(index->racine->filsGauche));
                if (delta < -1 || delta > 1){
                    printf("L'index n'est pas equilibre (delta = %i).\n", delta);
                } else {
                    printf("L'index est equilibre (delta = %i).\n", delta);
                }
                break;
            case 3:
                if (a_indexe == 0) {
                printf("\nAucun fichier n'est indexe.\n");
                break;
            }
                afficher_index(index);
                break;
            case 4:
                if (a_indexe == 0) {
                    printf("\nAucun fichier n'est indexe.\n");
                    break;
                }
                printf("\nEntrer le mot a chercher : ");
                char mot;
                scanf("%s", &mot);
                t_Noeud *noeud = rechercher_mot(index, &mot);
                if (noeud == NULL) {
                    printf("Mot introuvable.");
                } else {
                    t_Position *position = noeud->positions.debut;
                    while (position != NULL) {
                        printf("\n| - - - (l:%i, o:%i, p:%i)", position->numero_ligne, position->ordre, position->numero_phrase);
                        position = position->suivant;
                    }
                }
                break;
            case 5:
                if (a_indexe == 0) {
                    printf("\nAucun fichier n'est indexe.\n");
                    break;
                }
                afficher_max_apparition(index);
                break;
            case 6:
                if (a_indexe == 0) {
                    printf("\nAucun fichier n'est indexe.\n");
                    break;
                }
                printf("\nEntrer le mot a chercher : ");
                char * entree;
                scanf("%s", entree);
                printf("\nMot = \"%s\"\n", entree);
                afficher_occurences_mot(index, entree);
                break;
            case 7:
                if (a_indexe == 0) {
                    printf("\nAucun fichier n'est indexe.\n");
                    break;
                }
                printf("\nNom du fichier dans lequel enregistrer le texte : ");
                char filename;
                scanf("%s", &filename);
                construire_texte(index, "/Users/pol/Desktop/UTC/TC04/NF16/Algos/NF16/NF16-TP4/NF16-TP4/sortie.txt");
                break;
            case 8:
                printf("\nFermeture du programme");
                test = 0;
                break;
            default:
                break;
        }
    }
}
