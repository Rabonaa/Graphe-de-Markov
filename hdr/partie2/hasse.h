#ifndef __HASSE_H__
#define __HASSE_H__

#include "../hdr/partie2/etape1.h"

typedef struct {
    int from;
    int to;
}t_link;

typedef struct {
    t_link* links;
    int log_size;
}t_link_array;

t_link_array creer_hasse(t_liste_adj* graphe, t_part partition);
void removeTransitiveLinks(t_link_array *p_link_array);
void afficherHasse(t_link_array* liens);
void PrintGraphProperties(t_part partition, t_link_array links);


/*
 * @brief Creates a link array from the given partition and graph.
 *
 * @param part The partition of the graph.
 * @param graph The adjacency list representation of the graph.
 * @return The created link array.
*/

#endif