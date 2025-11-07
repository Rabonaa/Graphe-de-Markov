#ifndef __UTILS_H__
#define __UTILS_H__

//cellule qui représente une arête
typedef struct cellule{
    int sommet_arrivee;
    float probabilite;
    struct cellule *next;
}t_cell;


//liste qui stocke toutes les arêtes sortant d'un sommet
typedef struct liste {
    t_cell *head;
}t_list;

//liste d'adjacence (tableau dynamique de liste)
typedef struct ListeAdj {
    t_list *list;
    int taille;
}t_liste_adj;



t_cell* CreateCell(int sommet_arrivee, float proba);
void AddCell( t_list *list, int sommet_arrivee, float proba);
t_list EmptyList();
void DisplayList(t_list *list);
t_liste_adj EmptyAdjList(int taille);
void DisplayAdjList(t_liste_adj list_adj, int taille);

t_liste_adj readGraph(const char *filename);

void WriteGraph(const char *filename, t_liste_adj liste_adj);

void isMarkovGraph(t_liste_adj liste_adj);
#endif