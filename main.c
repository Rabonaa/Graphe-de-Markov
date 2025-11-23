#include <stdio.h>
#include <stdlib.h>
#include "hasse.h"
#include "matrix.h"
#include "utils.h"

int main() {
    t_liste_adj liste_adj = readGraph("./data/exemple_valid_step3.txt");
    DisplayAdjList(liste_adj, liste_adj.taille);
    isMarkovGraph(liste_adj);
    WriteGraph("./data/graph.txt", liste_adj);
    t_part partition = Tarjan(liste_adj);
    DisplayPartition(partition);

    t_link_array liens = FindInterClassLinks(liste_adj, partition);

    printf("\nLiens inter-classes trouves %d :\n", liens.log_size);
    for (int i = 0; i < liens.log_size; i++) {

        printf("  C%d -> C%d\n", liens.links[i].from + 1, liens.links[i].to + 1);
    }

    WriteHasseDiagram("hasse_complet.txt", partition, liens);
    printf("\nSuppression des redondances\n");
    removeTransitiveLinks(&liens);

    printf("Liens restants apres reduction %d :\n", liens.log_size);
    for (int i = 0; i < liens.log_size; i++) {
        printf("  C%d -> C%d\n", liens.links[i].from + 1, liens.links[i].to + 1);
    }


    WriteHasseDiagram("hasse_reduit.txt", partition, liens);
    PrintGraphProperties(partition, liens);

    t_matrix M = AdjacencyToMatrix(liste_adj);
    printf("\nMatrice de transition M :\n");
    DisplayMatrix(M);

    printf("\n--- Convergence Globale ---\n");
    t_matrix Mn = CreateZeroMatrix(M.rows, M.cols);
    CopyMatrix(Mn, M);

    int iter = 1;
    float diff = 1.0;
    t_matrix M_prev = CreateZeroMatrix(M.rows, M.cols);

    while (diff > 0.01 && iter < 100) {
        CopyMatrix(M_prev, Mn);
        t_matrix temp = MultiplyMatrices(Mn, M);
        FreeMatrix(Mn);
        Mn = temp;

        diff = DiffMatrix(Mn, M_prev);
        iter++;
        if (iter == 3 || iter == 7) {
            printf("\nMatrice M^%d :\n", iter);
            DisplayMatrix(Mn);
        }
    }

    if (diff <= 0.01) printf("\n> Convergence atteinte a n=%d (diff=%.4f)\n", iter, diff);
    else printf("\n> Pas de convergence stricte globale.\n");


    // 3. Analyse par Classe (Sous-matrices & Periodes)
    printf("\n--- Analyse Detaillee par Classe ---\n");

    for (int i = 0; i < partition.nb_classe; i++) {
        int is_transitive = 0;
        for (int k = 0; k < liens.log_size; k++) {
            if (liens.links[k].from == i) {
                is_transitive = 1;
                break;
            }
        }
        if (!is_transitive) {
            printf("\n[Classe C%d] (Persistante) :\n", i + 1);
            t_matrix Sub = SubMatrix(M, partition, i);
            int periode = GetPeriod(Sub);
            printf("  > Periode : %d\n", periode);
            t_matrix Sub_n = CreateZeroMatrix(Sub.rows, Sub.cols);
            CopyMatrix(Sub_n, Sub);
            t_matrix Sub_prev_iter = CreateZeroMatrix(Sub.rows, Sub.cols);
            float d = 1.0;
            int sub_iter = 0;
            while (d > 0.0001 && sub_iter < 200) {
                CopyMatrix(Sub_prev_iter, Sub_n);
                t_matrix res = MultiplyMatrices(Sub_n, Sub);
                FreeMatrix(Sub_n);
                Sub_n = res;
                d = DiffMatrix(Sub_n, Sub_prev_iter);
                sub_iter++;
            }
            printf("  > Distribution stationnaire :\n    ");
            for (int c = 0; c < Sub_n.cols; c++) printf("%.3f ", Sub_n.data[0][c]);
            printf("\n");
            FreeMatrix(Sub);
            FreeMatrix(Sub_n);
            FreeMatrix(Sub_prev_iter);
        }
    }
    free(liens.links);
    FreeMatrix(M);
    FreeMatrix(Mn);
    FreeMatrix(M_prev);
    return 0;
}
