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

    t_link_array links = BuildHasseLinks(liste_adj, partition);
    printf("\nLiens Hasse avant suppression des transitive links :\n");
    DisplayHasse(links, partition);

    removeTransitiveLinks(&links);
    printf("\nLiens Hasse apres suppression des transitive links :\n");
    DisplayHasse(links, partition);

    free(links.links);
    for (int i = 0; i < partition.nb_classe; i++) {
        free(partition.classes[i].sommets);
        free(partition.classes[i].nom);
    }
    free(partition.classes);
    free(liste_adj.list);
    return 0;
}
