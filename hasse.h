#ifndef __HASSE_H__
#define __HASSE_H__
#include "utils.h"

typedef struct t_link {
    int from;
    int to;
} t_link;

typedef struct t_link_array {
    t_link *links;
    int log_size;
    int capacite;
} t_link_array;

void removeTransitiveLinks(t_link_array *p_link_array);

/**
 * @brief Creates a link array from the given partition and graph.
 *
 * @param part The partition of the graph.
 * @param graph The adjacency list representation of the graph.
 * @return The created link array.
 */
t_link_array InitLinkArray(int capacite_initiale);
void AddLink(t_link_array *arr, int from, int to);
int *CreateVertexClassMap(t_part partition, int nb_sommets);
t_link_array BuildHasseLinks(t_liste_adj liste_adj, t_part partition);
void DisplayHasse(t_link_array links, t_part partition);
#endif