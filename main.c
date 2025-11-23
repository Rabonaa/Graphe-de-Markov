#include <stdio.h>
#include <stdlib.h>
#include "hasse.h"
#include "utils.h"

int main() {
    t_liste_adj liste_adj = readGraph("./data/exemple_valid_step3.txt");
    DisplayAdjList(liste_adj, liste_adj.taille);
    isMarkovGraph(liste_adj);
    WriteGraph("./data/graph.txt", liste_adj);
    t_part partition = Tarjan(liste_adj);
    DisplayPartition(partition);

    t_link_array liens = FindInterClassLinks(liste_adj, partition);

    printf("\nLiens inter-classes trouves %d :\n", liens.log_size);
    for (int i = 0; i < liens.log_size; i++) {

        printf("  C%d -> C%d\n", liens.links[i].from + 1, liens.links[i].to + 1);
    }

    WriteHasseDiagram("hasse_complet.txt", partition, liens);
    printf("\nSuppression des redondances\n");
    removeTransitiveLinks(&liens);

    printf("Liens restants apres reduction %d :\n", liens.log_size);
    for (int i = 0; i < liens.log_size; i++) {
        printf("  C%d -> C%d\n", liens.links[i].from + 1, liens.links[i].to + 1);
    }


    WriteHasseDiagram("hasse_reduit.txt", partition, liens);

    free(liens.links);
    return 0;
}
