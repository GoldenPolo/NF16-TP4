#include "tp4.h"

void main(){
    t_Index * index = creer_index();
    
    int i = indexer_fichier(index, "/Users/pol/Desktop/UTC/TC04/NF16/Algos/NF16/NF16-TP4/NF16-TP4/test.txt");
    
    afficher_index(index);
    afficher_max_apparition(index);
    
}
