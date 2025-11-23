#include <stdlib.h>

#include "partie1/cglm.h"
#include "partie2/etape1.h"
#include "partie2/hasse.h"

int main() {
    t_liste_adj liste_adj = readGraph("../data/exemple_valid_step3.txt");
    DisplayAdjList(liste_adj, liste_adj.taille);
    isMarkovGraph(liste_adj);
    WriteGraph("graph.txt", liste_adj);
    t_part partition= Tarjan(liste_adj);
    DisplayPartition(partition);
    t_link_array liens=creer_hasse(&liste_adj,partition);
    afficherHasse(&liens);
    return 0;
}
