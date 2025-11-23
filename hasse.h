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
    int taille;
} t_link_array;


t_link_array InitLinkArray(int capacite_initiale);
void AddLink(t_link_array *array, int from, int to);
int HasLink(t_link_array *array, int from, int to);
int *GetVertexToClassMapping(t_part partition, int nb_sommets);
t_link_array FindInterClassLinks(t_liste_adj liste_adj, t_part partition);
void WriteHasseDiagram(const char *filename, t_part partition, t_link_array links);
void removeTransitiveLinks(t_link_array *p_link_array);
/**
 * @brief Creates a link array from the given partition and graph.
 *
 * @param part The partition of the graph.
 * @param graph The adjacency list representation of the graph.
 * @return The created link array.
 */
#endif