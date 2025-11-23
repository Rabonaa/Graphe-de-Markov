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
            printf("\nCe n'est pas un graphe de Markov.\n");
            printf("La somme des probabilites du sommet %d est %.2f\n", i + 1, sum);
            return;
        }
    }
    printf("\nLe graphe est un graphe de Markov.\n\n");
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

//Fonction Pile----------------------------------------------------------------------------------------
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


