#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

static char *getID(int i)
{
    // translate from 1,2,3, .. ,500+ to A,B,C,..,Z,AA,AB,...
    static char buffer[10];
    char temp[10];
    int index = 0;

    i--; // Adjust to 0-based index
    while (i >= 0)
    {
        temp[index++] = 'A' + (i % 26);
        i = (i / 26) - 1;
    }

    // Reverse the string to get the correct order
    for (int j = 0; j < index; j++)
    {
        buffer[j] = temp[index - j - 1];
    }
    buffer[index] = '\0';

    return buffer;
}

//Création d'une cellule
t_cell* CreateCell(int sommet_arrivee, float proba) {
    t_cell *cell = malloc(sizeof(t_cell));
    if (!cell) {
        printf("Erreur d'allocation!\n");
        exit(EXIT_FAILURE);
    }
    cell->sommet_arrivee = sommet_arrivee;
    cell->probabilite = proba;
    cell->next = NULL;
    return cell;
}

//Création d'une liste vide
t_list EmptyList() {
    t_list list;
    list.head = NULL;
    return list;
}

//Ajouter une cellule à une liste
void AddCell( t_list *list, int sommet_arrivee, float proba) {
    t_cell *new_cell = CreateCell(sommet_arrivee, proba);
    new_cell->next = list->head;
    list->head = new_cell;
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
    printf("@-> NULL\n");
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

//affichage d'une liste d'adjacene(1)
/*void DisplayAdjList(t_liste_adj list_adj, int taille) {
    if (!list_adj.list) {
        printf("Liste d'adjacence vide!\n");
        return;
    }

    for (int i = 1; i <= taille; i++) {
        printf("Liste pour le sommet %d: [head @] -> ",i);
        for (int i = 1; i <= taille; i++) {
            printf("(.., ..) @-> ", list_adj.list[i]);
        }

    }
}*/

//affichage d'une liste d'adjacene(2)
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