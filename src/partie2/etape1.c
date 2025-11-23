#include <stdio.h>
#include <stdlib.h>

#include "partie2/etape1.h"

#include <string.h>
bool isEmptyStack(t_stack *stack) {
    if (!stack->s_list.head) {
        return 1;
    }
    return 0;
}

void push(t_stack *stack, int val) {
    t_scell *new_cell = malloc(sizeof(t_scell));
    if (!new_cell) {
        printf("Erreur d'allocation memoire\n");
        exit(EXIT_FAILURE);
    }
    new_cell->sommet = val;
    new_cell->next = stack->s_list.head;
    stack->s_list.head = new_cell;
}

int pop(t_stack *stack) {
    if (isEmptyStack(stack)) {
        return -1;
    }
    t_scell *head = stack->s_list.head;
    int sommet = head->sommet;
    stack->s_list.head = head->next;
    free(head);
    return sommet;
}
//----------------------------------------------------------------------------------------


t_tarjan_vertex *InitTarjanVertices(t_liste_adj liste_adj) {
    t_tarjan_vertex *tab = malloc(liste_adj.taille * sizeof(t_tarjan_vertex));
    if (!tab) {
        printf("Erreur d'allocation mémoire\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < liste_adj.taille; i++) {
        tab[i].id = i + 1;
        tab[i].num = -1;
        tab[i].low = -1;
        tab[i].test = 0;
    }
    return tab;
}

// Initialisation partition vide
t_part InitPartition(int capacite_initiale) {
    t_part partition;
    partition.nb_classe = 0;
    partition.taille = capacite_initiale;
    partition.classes = malloc(capacite_initiale * sizeof(t_classe));
    if (!partition.classes) {
        printf("Erreur d'allocation partition\n");
        exit(EXIT_FAILURE);
    }
    return partition;
}

// Ajout nouvelle classe dans partition
t_classe *AddClasseToPartition(t_part *partition, const char *nom) {
    if (partition->nb_classe >= partition->taille) {
        partition->taille *= 2;
        partition->classes = realloc(partition->classes, partition->taille * sizeof(t_classe));
        if (!partition->classes) {
            printf("Erreur de realloc partition\n");
            exit(EXIT_FAILURE);
        }
    }
    t_classe *cl = &partition->classes[partition->nb_classe++];
    cl->nom = strdup(nom);
    cl->nb_vertex = 0;
    cl->taille = 4; // capacité initiale
    cl->sommets = malloc(cl->taille * sizeof(int));
    if (!cl->sommets) {
        printf("Erreur d'allocation classe\n");
        exit(EXIT_FAILURE);
    }
    return cl;
}

// Ajout un sommet à une classe
void AddSommetToClasse(t_classe *cl, int sommet) {
    if (cl->nb_vertex >= cl->taille) {
        cl->taille *= 2;
        cl->sommets = realloc(cl->sommets, cl->taille * sizeof(int));
        if (!cl->sommets) {
            printf("Erreur de realloc classe\n");
            exit(EXIT_FAILURE);
        }
    }
    cl->sommets[cl->nb_vertex++] = sommet;
}

//Algo de Tarjan
void Parcours(int u, t_liste_adj liste_adj, t_tarjan_vertex *tab_vertex,
                t_stack *stack, t_part *partition, int *index) {

    tab_vertex[u].num = tab_vertex[u].low = (*index)++;
    push(stack, u);
    tab_vertex[u].test = 1;
    t_cell *curr = liste_adj.list[u].head;
    while (curr) {
        int v = curr->sommet_arrivee - 1;
        if (tab_vertex[v].num == -1) {
            Parcours(v, liste_adj, tab_vertex, stack, partition, index);
            if (tab_vertex[v].low < tab_vertex[u].low)
                tab_vertex[u].low = tab_vertex[v].low;
        } else if (tab_vertex[v].test) {
            if (tab_vertex[v].num < tab_vertex[u].low)
                tab_vertex[u].low = tab_vertex[v].num;
        }
        curr = curr->next;
    }
    if (tab_vertex[u].low == tab_vertex[u].num) {
        char nom[16];
        sprintf(nom, "Composante C%d", partition->nb_classe + 1);
        t_classe *cl = AddClasseToPartition(partition, nom);

        int w;
        do {
            w = pop(stack);
            tab_vertex[w].test = 0;
            AddSommetToClasse(cl, tab_vertex[w].id);
        } while (w != u);
    }
}

//Fonction Tarjan
t_part Tarjan(t_liste_adj liste_adj) {
    t_tarjan_vertex *tab_vertex = InitTarjanVertices(liste_adj);
    t_stack stack = {0};
    int index = 0;
    t_part partition = InitPartition(4);

    for (int i = 0; i < liste_adj.taille; i++) {
        if (tab_vertex[i].num == -1) {
            Parcours(i, liste_adj, tab_vertex, &stack, &partition, &index);
        }
    }

    free(tab_vertex);
    return partition;
}

//Afficher Partition
void DisplayPartition(t_part partition) {
    printf("Partition en %d classes :\n", partition.nb_classe);
    for (int i = 0; i < partition.nb_classe; i++) {
        t_classe cl = partition.classes[i];
        printf("%s : ", cl.nom);
        for (int j = 0; j < cl.nb_vertex; j++) {
            printf("%d ", cl.sommets[j]);
        }
        printf("\n");
    }
}