#include <stdio.h>
#include "utils.h"

int main() {
    t_liste_adj liste_adj = readGraph("exemple1.txt");
    DisplayAdjList(liste_adj, liste_adj.taille);
    isMarkovGraph(liste_adj);
    WriteGraph("Test_dessin", liste_adj);
    return 0;
}
