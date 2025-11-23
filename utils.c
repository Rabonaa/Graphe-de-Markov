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


//Ajouter une cellule à une liste
void AddCell( t_list *list, int sommet_arrivee, float proba) {
    t_cell *new_cell = CreateCell(sommet_arrivee, proba);
    new_cell->next = list->head;
    list->head = new_cell;
}

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

//lecture graphe
t_liste_adj readGraph(const char *filename) {
    FILE *file = fopen(filename, "rt");
    int nbsommet, depart, arrivee;
    float proba;
    t_liste_adj liste_adj;
    if (!file) {
        perror("Erreur de lecture du fichier\n");
        exit(EXIT_FAILURE);
    }
    if (fscanf(file, "%d", &nbsommet) != 1) {
        perror("Nombre de sommet introuvable\n");
        exit(EXIT_FAILURE);
    }
    liste_adj = EmptyAdjList(nbsommet);
    while (fscanf(file, "%d %d %f", &depart, &arrivee, &proba) == 3) {
        if (depart < 1 || depart > nbsommet) {
            perror("Depart invalide\n");
            continue;
        }
        else {
            AddCell(&liste_adj.list[depart - 1], arrivee, proba);
        }
    }
    fclose(file);
    return liste_adj;
}


//Verification de graphe
void isMarkovGraph(t_liste_adj liste_adj) {
    if (!liste_adj.list) {
        return ;
    }
    for (int i = 0; i < liste_adj.taille; i++) {
        float sum = 0;
        t_cell *curr = liste_adj.list[i].head;
        while (curr) {
            float proba = curr->probabilite;
            sum += proba;
            curr = curr->next;
        }
        if (sum < 0.99 || sum > 1) {
            printf("Ce n'est pas un graphe de Markov.\n");
            printf("La somme des probabilites du sommet %d est %.2f\n", i + 1, sum);
            return;
        }
    }
    printf("Le graphe est un graphe de Markov.");
}

//Graphe en txt
void WriteGraph(const char *filename, t_liste_adj liste_adj) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Erreur d'ouverture du fichier.\n");
        return;
    }
    fprintf(file,"---\nconfig:\n");
    fprintf(file, "  layout: elk\n");
    fprintf(file, "  theme: neo\n");
    fprintf(file, "  look: neo\n---\n");
    fprintf(file, "\nflowchart LR\n");
    for (int i = 0; i < liste_adj.taille; i++) {
        const char *id = getID(i + 1);
        fprintf(file, "%s((%d))\n", id, i + 1);
    }
    fprintf(file, "\n");
    for (int i = 0; i < liste_adj.taille; i++) {
        t_cell *curr = liste_adj.list[i].head;
        while (curr) {
            const char *id = getID(i + 1);
            fprintf(file, "%s", id);
            const char *id_sommet_arrivee = getID(curr->sommet_arrivee);
            fprintf(file, " -->|%.2f|%s\n", curr->probabilite, id_sommet_arrivee);
            curr = curr->next;
        }
    }
    fclose(file);
}

//Sous-Fonction pour Algo de Tarjan
t_vertex *Tarjan_func(t_liste_adj liste_adj) {
    t_vertex *tab_vertex = malloc(liste_adj.taille * sizeof(tab_vertex));
    if (tab_vertex == NULL) {
        printf( "Erreur d'allocation memoire\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < liste_adj.taille; i++) {
        t_tarjan_vertex tarjan_vertex = {i + 1, -1, -1, 0};
        tab_vertex[i].tarjan_vertex = tarjan_vertex;
    }
    return tab_vertex;
}

//Pile


