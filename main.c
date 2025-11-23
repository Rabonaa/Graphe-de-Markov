#include "partie1/cglm.h"
#include "partie2/etape1.h"

int main() {
    t_liste_adj liste_adj = readGraph("../data/exemple_valid_step3.txt");
    DisplayAdjList(liste_adj, liste_adj.taille);
    isMarkovGraph(liste_adj);
    WriteGraph("graph.txt", liste_adj);
    t_partition* partition=tarjan(liste_adj);
    affichage_partition(*partition);
    return 0;
}
