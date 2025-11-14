#include "partie1/cglm.h"

int main() {
    t_liste_adj liste_adj = readGraph("../exemple_valid_step3.txt");
    DisplayAdjList(liste_adj, liste_adj.taille);
    isMarkovGraph(liste_adj);
    WriteGraph("graph.txt", liste_adj);
    return 0;
}
