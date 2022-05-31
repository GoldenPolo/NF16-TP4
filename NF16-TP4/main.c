#include "tp4.h"

void main(){
    t_Index * index = creer_index();
    
    int i = indexer_fichier(index, "/Users/pol/Desktop/UTC/TC04/NF16/Algos/NF16/NF16-TP4/NF16-TP4/test.txt");
    
    afficher_index(index);
    afficher_max_apparition(index);
    
    int test = 1;

    while (test == 1) {
        printf("\n\nMENU\n-----\n1 = charger un fichier\n2 = caracteristiques de l'index\n3 = afficher index\n"
               "4 = rechercher un mot\n5 = afficher le mot avec le maximum d'apparitions\n"
               "6 = afficher les occurrences d'un mot\n7 = construire le texte a partir de l'index\n8 = quitter\n");
        printf("Votre choix:");
        int choix;
        scanf("%i", &choix);
        t_Index *index = creer_index();

        switch (choix) {
            case 1:
                printf("\nNom du fichier:");
                char nom_fichier;
                scanf("%s", &nom_fichier);
                // "C:\\Users\\alexa\\Documents\\UTC\\TC03 - P22\\NF16\\TP\\TP4\\TP4\\"
                int i = indexer_fichier(index, &nom_fichier);
                printf("\nNombre de mots lus dans le fichier = %i", i);
                break;
            case 2:
                // ??
                break;
            case 3:
                afficher_index(index);
                break;
            case 4:
                printf("\nEntrer le mot a chercher:");
                char mot;
                scanf("%s", &mot);
                t_Noeud *noeud = rechercher_mot(index, &mot);
                t_Position *position = noeud->positions.debut;
                while (position != NULL) {
                    printf("\n| - - - (l:%i, o:%i, p:%i)", position->numero_ligne, position->ordre, position->numero_phrase);
                    position = position->suivant;
                }
                break;
            case 5:
                afficher_max_apparition(index);
                break;
            case 6:
                printf("\nEntrer le mot:");
                char le_mot;
                scanf("%s", &le_mot);
                afficher_occurrences_mot(index, &le_mot);
                break;
            case 7:
                printf("\nNom du fichier dans lequel enregistrer le texte:");
                char filename;
                scanf("%s", &filename);
                // "C:\\Users\\alexa\\Documents\\UTC\\TC03 - P22\\NF16\\TP\\TP4\\TP4\\"
                //construire_texte(index, &filename);
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
