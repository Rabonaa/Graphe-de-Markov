#ifndef TARJAN_H
#define TARJAN_H

#include <stdbool.h>

#include "../hdr/partie1/cglm.h"
//Type Tarjan
typedef struct t_tarjan_vertex {
    int id;
    int num;
    int low;
    int test;
}t_tarjan_vertex;

//Classe
typedef struct t_classe {
    char *nom;
    int *sommets;
    int nb_vertex;
    int taille;
} t_classe;

//Partition
typedef struct t_part {
    t_classe *classes;
    int nb_classe;
    int taille;
} t_part;

//Type pile
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

//Fonction Pile
void push(t_stack *stack, int val);
int pop(t_stack *stack);
bool isEmptyStack(t_stack *stack);

t_tarjan_vertex *InitTarjanVertices(t_liste_adj liste_adj);
t_part InitPartition(int capacite_initiale);
t_classe *AddClasseToPartition(t_part *partition, const char *nom);
void AddSommetToClasse(t_classe *cl, int sommet);
void Parcours(int u, t_liste_adj liste_adj, t_tarjan_vertex *tab_vertex,
                t_stack *stack, t_part *partition, int *index);
t_part Tarjan(t_liste_adj liste_adj);
void DisplayPartition(t_part partition);

#endif // TARJAN_H