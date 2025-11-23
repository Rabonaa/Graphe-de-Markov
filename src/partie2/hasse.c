#include <malloc.h>
#include "../hdr/partie2/hasse.h"

#include <stdio.h>

t_link_array creer_hasse(t_liste_adj* graphe, t_part partition) {
    t_link_array hasse;
    hasse.links=NULL;
    hasse.log_size=0;
    int* itoC=malloc((graphe->taille+1)*sizeof(int));
    for (int i=0;i<partition.nb_classe;i++) {
        for (int j=0;j<partition.classes[i].nb_vertex;j++) {
            int sommet_original = partition.classes[i].sommets[j];
            itoC[sommet_original] = i ;
        }
    }
    for (int i=1 ; i<graphe->taille;i++) {
        t_cell* temp=graphe->list[i].head;
        while (temp !=NULL) {
            int classe_depart = itoC[i];
            int classe_arrivee = itoC[temp->sommet_arrivee];
            if (classe_arrivee != classe_depart) {
                bool existe = 0;
                for (int j=0; j<hasse.log_size;j++) {
                    if (hasse.links[j].from == classe_depart && hasse.links[j].to == classe_arrivee) {
                        existe=1;
                    }
                }
                if (!existe) {
                    hasse.log_size++;
                    hasse.links = (t_link*)realloc(hasse.links, hasse.log_size * sizeof(t_link));
                    hasse.links[hasse.log_size-1].from = classe_depart;
                    hasse.links[hasse.log_size-1].to = classe_arrivee;
                }
            }
            temp = temp->next;
        }
    }
    free(itoC);
    removeTransitiveLinks(&hasse);
    return hasse;
}


void removeTransitiveLinks(t_link_array *p_link_array) {
    int i, j, k;
    for (i = 0; i < p_link_array->log_size; i++) {
        for (j = 0; j < p_link_array->log_size; j++) {
            if (p_link_array->links[i].to == p_link_array->links[j].from) {
                int a = p_link_array->links[i].from;
                int c = p_link_array->links[j].to;
                for (k = 0; k < p_link_array->log_size; k++) {
                    if (p_link_array->links[k].from == a && p_link_array->links[k].to == c) {
                        p_link_array->links[k] = p_link_array->links[p_link_array->log_size - 1];
                        p_link_array->log_size--;
                        k--;
                    }
                }
            }
        }
    }
}

void afficherHasse(t_link_array* liens) {
    printf("\n--- Diagramme de Hasse ---\n");
    if (liens->log_size == 0) printf("Aucun lien.\n");
    for (int i = 0; i < liens->log_size; i++) {
        printf("C%d -> C%d\n", liens->links[i].from + 1, liens->links[i].to + 1);
    }
}

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