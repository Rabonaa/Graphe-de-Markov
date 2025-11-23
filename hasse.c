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

// Initialisation du tableau de liens
t_link_array InitLinkArray(int capacite_initiale) {
    t_link_array array;
    array.log_size = 0;
    array.taille = capacite_initiale;
    array.links = malloc(capacite_initiale * sizeof(t_link));
    if (!array.links) {
        printf("Erreur d'allocation pour les liens\n");
        exit(EXIT_FAILURE);
    }
    return array;
}

// Ajout d'un lien dans le tableau
void AddLink(t_link_array *array, int from, int to) {
    if (array->log_size >= array->taille) {
        array->taille *= 2;
        array->links = realloc(array->links, array->taille * sizeof(t_link));
        if (!array->links) {
            printf("Erreur de realloc liens\n");
            exit(EXIT_FAILURE);
        }
    }
    array->links[array->log_size].from = from;
    array->links[array->log_size].to = to;
    array->log_size++;
}

// Vérifie si un lien existe déjà pour éviter les doublons immédiats
int HasLink(t_link_array *array, int from, int to) {
    for (int i = 0; i < array->log_size; i++) {
        if (array->links[i].from == from && array->links[i].to == to) {
            return 1;
        }
    }
    return 0;
}

//index sommet -> index classe
int *GetVertexToClassMapping(t_part partition, int nb_sommets) {
    int *mapping = malloc(nb_sommets * sizeof(int));
    if (!mapping) exit(EXIT_FAILURE);

    for (int i = 0; i < partition.nb_classe; i++) {
        t_classe cl = partition.classes[i];
        for (int j = 0; j < cl.nb_vertex; j++) {
            if (cl.sommets[j] - 1 < nb_sommets) {
                mapping[cl.sommets[j] - 1] = i;
            }
        }
    }
    return mapping;
}

t_link_array FindInterClassLinks(t_liste_adj liste_adj, t_part partition) {
    int *vertex_to_class = GetVertexToClassMapping(partition, liste_adj.taille);
    t_link_array links = InitLinkArray(10);
    for (int i = 0; i < liste_adj.taille; i++) {
        int Ci = vertex_to_class[i];
        t_cell *curr = liste_adj.list[i].head;
        while (curr) {
            int j = curr->sommet_arrivee - 1;
            int Cj = vertex_to_class[j];
            if (Ci != Cj) {
                if (!HasLink(&links, Ci, Cj)) {
                    AddLink(&links, Ci, Cj);
                }
            }
            curr = curr->next;
        }
    }
    free(vertex_to_class);

    return links;
}

//Diagramme
void WriteHasseDiagram(const char *filename, t_part partition, t_link_array links) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Erreur ouverture fichier Hasse");
        return;
    }
    fprintf(file,"---\nconfig:\n");
    fprintf(file, "  layout: elk\n");
    fprintf(file, "  theme: neo\n");
    fprintf(file, "  look: neo\n---\n");
    fprintf(file, "\nflowchart LR\n");
    for (int i = 0; i < partition.nb_classe; i++) {
        t_classe cl = partition.classes[i];
        fprintf(file, "C%d[\"{", i + 1);
        for(int j = 0; j < cl.nb_vertex; j++) {
            fprintf(file, "%d", cl.sommets[j]);
            if(j < cl.nb_vertex - 1) fprintf(file, ",");
        }
        fprintf(file, "}\"]\n");
    }
    fprintf(file, "\n");

    for (int i = 0; i < links.log_size; i++) {
        fprintf(file, "C%d --> C%d\n", links.links[i].from + 1, links.links[i].to + 1);
    }

    fclose(file);
}

//Propriétés graphe
void PrintGraphProperties(t_part partition, t_link_array links) {
    printf("\nCARACTERISTIQUES DU GRAPHE\n");

    if (partition.nb_classe == 1) {
        printf("-> Le graphe est irreductible.\n");
    } else {
        printf("-> Le graphe n'est pas irreductible (%d classes).\n", partition.nb_classe);
    }

    for (int i = 0; i < partition.nb_classe; i++) {
        t_classe cl = partition.classes[i];
        int is_transitive = 0;

        for (int k = 0; k < links.log_size; k++) {
            if (links.links[k].from == i) {
                is_transitive = 1;
                break;
            }
        }

        printf(" La classe C%d {", i + 1);
        for (int j = 0; j < cl.nb_vertex; j++) {
            printf("%d", cl.sommets[j]);
            if (j < cl.nb_vertex - 1) printf(",");
        }
        printf("} est ");

        if (is_transitive) {
            printf("transitoire\n");
            printf("   -> Les etats { ");
            for (int j = 0; j < cl.nb_vertex; j++) printf("%d ", cl.sommets[j]);
            printf("} sont transitoires.\n");
        } else {
            printf("persistante\n");
            printf("   -> Les etats { ");
            for (int j = 0; j < cl.nb_vertex; j++) printf("%d ", cl.sommets[j]);
            printf("} sont persistants.\n");

            if (cl.nb_vertex == 1) {
                printf("   -> L'etat %d est absorbant.\n", cl.sommets[0]);
            }
        }
        printf("\n");
    }
}