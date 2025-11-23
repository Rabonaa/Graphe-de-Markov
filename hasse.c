#include <malloc.h>
#include "hasse.h"
#include <stdio.h>
#include "utils.h"


void removeTransitiveLinks(t_link_array *p_link_array)
{
    int i = 0;
    while (i < p_link_array->log_size)
    {
        t_link link1 = p_link_array->links[i];
        int j = 0;
        int to_remove = 0;
        while (j < p_link_array->log_size && !to_remove)
        {
            if (j != i)
            {
                t_link link2 = p_link_array->links[j];
                if (link1.from == link2.from)
                {
                    // look for a link from link2.to to link1.to
                    int k = 0;
                    while (k < p_link_array->log_size && !to_remove)
                    {
                        if (k != j && k != i)
                        {
                            t_link link3 = p_link_array->links[k];
                            if ((link3.from == link2.to) && (link3.to == link1.to))
                            {
                                to_remove = 1;
                            }
                        }
                        k++;
                    }
                }
            }
            j++;
        }
        if (to_remove)
        {
            // remove link1 by replacing it with the last link
            p_link_array->links[i] = p_link_array->links[p_link_array->log_size - 1];
            p_link_array->log_size--;
        }
        else
        {
            i++;
        }
    }
}

t_link_array InitLinkArray(int capacite_initiale) {
    t_link_array arr;
    arr.links = malloc(capacite_initiale * sizeof(t_link));
    if (!arr.links) {
        printf("Erreur allocation link array\n");
        exit(EXIT_FAILURE);
    }
    arr.log_size = 0;
    arr.capacite = capacite_initiale;
    return arr;
}

//Ajout lien
void AddLink(t_link_array *arr, int from, int to) {
    for (int i = 0; i < arr->log_size; i++) {
        if (arr->links[i].from == from && arr->links[i].to == to)
            return;
    }
    if (arr->log_size >= arr->capacite) {
        arr->capacite *= 2;
        arr->links = realloc(arr->links, arr->capacite * sizeof(t_link));
        if (!arr->links) {
            printf("Erreur realloc link array\n");
            exit(EXIT_FAILURE);
        }
    }
    arr->links[arr->log_size].from = from;
    arr->links[arr->log_size].to = to;
    arr->log_size++;
}

//Tableau sommet->classe
int *CreateVertexClassMap(t_part partition, int nb_sommets) {
    int *map = malloc(nb_sommets * sizeof(int));
    if (!map) {
        printf("Erreur d'allocation vertex\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < nb_sommets; i++)
        map[i] = -1;

    for (int ci = 0; ci < partition.nb_classe; ci++) {
        t_classe cl = partition.classes[ci];
        for (int vi = 0; vi < cl.nb_vertex; vi++) {
            int sommet = cl.sommets[vi];
            map[sommet-1] = ci;
        }
    }
    return map;
}

//Liens classes
t_link_array BuildHasseLinks(t_liste_adj liste_adj, t_part partition) {
    int *vertex_class = CreateVertexClassMap(partition, liste_adj.taille);
    t_link_array links = InitLinkArray(4);

    for (int u = 0; u < liste_adj.taille; u++) {
        int cu = vertex_class[u];
        t_cell *curr = liste_adj.list[u].head;
        while (curr) {
            int v = curr->sommet_arrivee - 1;
            int cv = vertex_class[v];
            if (cu != cv) {
                AddLink(&links, cu, cv);
            }
            curr = curr->next;
        }
    }

    free(vertex_class);
    return links;
}

//Affichage Hasse
void DisplayHasse(t_link_array links, t_part partition) {
    printf("flowchart LR\n");
    for (int i = 0; i < links.log_size; i++) {
        int dep = links.links[i].from;
        int arr = links.links[i].to;
        printf("%s --> %s\n", partition.classes[dep].nom, partition.classes[arr].nom);
    }
}
