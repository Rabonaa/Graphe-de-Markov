#ifndef ETAPE1_H
#define ETAPE1_H

#include <stdbool.h>

#include "partie1/utils.h"
#include "partie1/cglm.h"

//* info pour sur chaque sommet
typedef struct tarjan_vertex{
    int identifiant;
    int num;
    int num_acces;
    bool indic;
}t_tarjan_vertex;

typedef t_tarjan_vertex** tab_tarjan;

typedef struct classe {
    int nom;
    tab_tarjan liste;
    int taille;
}t_classe;

typedef struct partition {
    t_classe** liste;
    int nb_classes;
}t_partition;

tab_tarjan graph_tab(t_liste_adj liste_adj);

typedef struct stack_cell {
    int sommet;
    struct stack_cell *next;
}t_scell;

typedef struct t_stack_list {
    t_scell *head;
}t_slist;

typedef struct t_stack {
    t_slist s_list;
}t_stack;

bool isEmptyStack(t_stack *stack);
void push(t_stack*,int);
int pop(t_stack*);

int min(int i1,int i2);

void parcours(int sommet,int* num,t_stack* pile,t_liste_adj graph,tab_tarjan tab,t_partition* partition);
t_partition* tarjan(t_liste_adj graph);

void affichage_partition(t_partition partition);

#endif //ETAPE1_H
