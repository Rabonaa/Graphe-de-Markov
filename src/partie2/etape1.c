#include <stdio.h>
#include <stdlib.h>

#include "partie2/etape1.h"

tab_tarjan graph_tab(t_liste_adj liste_adj) {
    if (liste_adj.taille==0) {
        printf("Liste vide.\n");
        return NULL;
    }
    tab_tarjan tab = malloc(liste_adj.taille*sizeof(t_tarjan_vertex*));
    if (!tab) {
        printf("Erreur d'allocation!\n");
        exit(EXIT_FAILURE);
    }
    for (int i=0; i<liste_adj.taille;i++) {
        t_tarjan_vertex* cel=malloc(sizeof(t_tarjan_vertex));
        cel->identifiant=i;
        cel->num = -1;
        cel->num_acces = -1;
        cel->indic = 0;
        tab[i]=cel;
    }
    return tab;
}

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

int min(int i1,int i2) {
    if (i1<=i2) {
        return i1;
    }
    return i2;
}

void parcours(int sommet,int* num,t_stack* pile,t_liste_adj graph,tab_tarjan tab,t_partition* partition) {
    tab[sommet]->num=*num;
    tab[sommet]->num_acces=*num;
    *num+=1;
    push(pile,sommet);
    tab[sommet]->indic=1;
    t_cell* temp=graph.list[sommet].head;
    int v;
    while (temp!=NULL) {
        v = temp->sommet_arrivee;
        if (tab[v]->num==-1) {
            parcours(v,num,pile,graph,tab,partition);
            tab[sommet]->num_acces= min(tab[sommet]->num_acces,tab[v]->num_acces);
        }
        else {
            if (tab[v]->indic==1) {
                tab[sommet]->num_acces= min(tab[sommet]->num_acces,tab[v]->num);
            }
        }
        temp=temp->next;
    }
    if (tab[sommet]->num_acces==tab[sommet]->num) {
        t_classe* new_classe=malloc(sizeof(t_classe));
        new_classe->taille=0;
        new_classe->liste=malloc(graph.taille*sizeof(t_tarjan_vertex*));
        do {
            v=pop(pile);
            tab[v]->indic=0;
            new_classe->liste[new_classe->taille]=tab[v];
            new_classe->taille++;
        }while (v!=sommet);
        new_classe->nom=sommet;
        partition->nb_classes++;
        partition->liste[partition->nb_classes-1]=new_classe;
    }
}

t_partition* tarjan(t_liste_adj graph) {
    tab_tarjan tab=graph_tab(graph);
    if (tab==NULL) {
        t_partition* partition = malloc(sizeof(t_partition));
        partition->liste = NULL;
        partition->nb_classes = 0;
        return partition;
    }
    t_stack* pile=malloc(sizeof(t_stack));
    t_partition* partition= malloc(sizeof(t_partition));
    partition->liste=malloc(graph.taille*sizeof(t_classe*));
    partition->nb_classes=0;
    int num=0;
    parcours(0,&num,pile,graph,tab,partition);
    for (int i=1; i<graph.taille;i++) {
        if (tab[i]->num==-1) {
            parcours(i,&num,pile,graph,tab,partition);
        }
    }
    free(pile);
    return partition;
}

void affichage_partition(t_partition partition) {
    printf("Nombre de classes: %d\n",partition.nb_classes);
    for (int i=0;i<partition.nb_classes;i++) {
        t_classe* classe = partition.liste[i];
        printf("Nombre de tarjan dans la classe %d\n",partition.liste[i]->taille);
        printf("Composante%d: {",classe->nom);
        for (int j=0;j<classe->taille;j++){
            printf("%d",classe->liste[j]->identifiant);
            if (j< classe->taille -1) {
                printf(", ");
            }
        }
        printf("}\n");
    }
}





