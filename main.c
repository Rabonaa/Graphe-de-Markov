#include <stdio.h>
#include "utils.h"

int main() {
    t_liste_adj liste_adj = readGraph("./data/exemple_valid_step3.txt");
    DisplayAdjList(liste_adj, liste_adj.taille);
    isMarkovGraph(liste_adj);
    WriteGraph("./data/graph.txt", liste_adj);
    t_part partition = Tarjan(liste_adj);
    DisplayPartition(partition);

    return 0;
}
