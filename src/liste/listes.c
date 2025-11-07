#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../utils.h"

#include "listes.h"



//Création d'une liste vide
t_list EmptyList() {
    t_list list;
    list.head = NULL;
    return list;
}

//Afficher une liste
void DisplayList(t_list *list) {
    if (!list->head) {
        printf("Liste vide.\n");
        return;
    }
    t_cell *head = list->head;
    t_cell *temp = head->next;
    printf("[head @] -> (%d, %.2f) ", head->sommet_arrivee, head->probabilite);
    while (temp) {
        printf("@-> (%d, %.2f) ", temp->sommet_arrivee, temp->probabilite);
        temp = temp->next;
    }
    printf("\n");
}

//Création d'une liste d'adjacence vide
t_liste_adj EmptyAdjList(int taille) {
    t_liste_adj list_adj;
    list_adj.list = malloc(taille * sizeof(t_list));
    if (!list_adj.list) {
        printf("Erreur d'allocation!\n");
        exit(EXIT_FAILURE);
    }
    list_adj.taille = taille;
    for (int i = 0; i < taille; i++) {
        list_adj.list[i] = EmptyList();
    }
    return list_adj;
}

//affichage d'une liste d'adjacene
void DisplayAdjList(t_liste_adj list_adj, int taille) {
    if (!list_adj.list) {
        printf("Liste d'adjacence vide!\n");
        return;
    }
    for (int i = 0; i < taille; i++) {
        printf("Liste pour le sommet %d: ",i + 1);
        DisplayList(&list_adj.list[i]);
    }
}
