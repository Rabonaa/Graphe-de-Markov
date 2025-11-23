//
// Created by jules on 23/11/2025.
//

#include "../hdr/partie3/matrix.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

//Creation Matrice Nulle
t_matrix CreateZeroMatrix(int rows, int cols) {
    t_matrix mat;
    mat.rows = rows;
    mat.cols = cols;
    mat.data = malloc(rows * sizeof(float *));
    if (!mat.data) exit(EXIT_FAILURE);
    for (int i = 0; i < rows; i++) {
        mat.data[i] = calloc(cols, sizeof(float));
        if (!mat.data[i]) exit(EXIT_FAILURE);
    }
    return mat;
}

//Liberation Memoire Matrice
void FreeMatrix(t_matrix mat) {
    for (int i = 0; i < mat.rows; i++) {
        free(mat.data[i]);
    }
    free(mat.data);
}

//Conversion Liste Adj en Matrice
t_matrix AdjacencyToMatrix(t_liste_adj list_adj) {
    int n = list_adj.taille;
    t_matrix mat = CreateZeroMatrix(n, n);

    for (int i = 0; i < n; i++) {
        t_cell *curr = list_adj.list[i].head;
        while (curr) {
            int col = curr->sommet_arrivee - 1;
            if (col >= 0 && col < n) {
                mat.data[i][col] = curr->probabilite;
            }
            curr = curr->next;
        }
    }
    return mat;
}

//Copie Matrice
void CopyMatrix(t_matrix dest, t_matrix src) {
    if (dest.rows != src.rows || dest.cols != src.cols) return;
    for (int i = 0; i < src.rows; i++) {
        for (int j = 0; j < src.cols; j++) {
            dest.data[i][j] = src.data[i][j];
        }
    }
}

//Affichage Matrice
void DisplayMatrix(t_matrix mat) {
    for (int i = 0; i < mat.rows; i++) {
        printf("| ");
        for (int j = 0; j < mat.cols; j++) {
            printf("%5.2f ", mat.data[i][j]);
        }
        printf("|\n");
    }
}

//Multiplication de Matrices
t_matrix MultiplyMatrices(t_matrix A, t_matrix B) {
    if (A.cols != B.rows) {
        printf("Erreur dimensions multiplication\n");
        exit(EXIT_FAILURE);
    }
    t_matrix res = CreateZeroMatrix(A.rows, B.cols);
    for (int i = 0; i < A.rows; i++) {
        for (int j = 0; j < B.cols; j++) {
            float sum = 0;
            for (int k = 0; k < A.cols; k++) {
                sum += A.data[i][k] * B.data[k][j];
            }
            res.data[i][j] = sum;
        }
    }
    return res;
}

//Difference Matrices
float DiffMatrix(t_matrix A, t_matrix B) {
    float diff = 0;
    for (int i = 0; i < A.rows; i++) {
        for (int j = 0; j < A.cols; j++) {
            diff += fabsf(A.data[i][j] - B.data[i][j]);
        }
    }
    return diff;
}

/**
 * @brief Calcule le PGCD (Plus Grand Commun Diviseur) d'un tableau d'entiers.
 * * Explication mathématique :
 * Cette fonction utilise l'algorithme d'Euclide.
 * La période d'une classe est le PGCD des longueurs de tous les cycles possibles
 * qui reviennent à un état de départ.
 * * @param vals Tableau contenant les longueurs des cycles trouvés.
 * @param nbvals Nombre d'éléments dans le tableau.
 * @return int Le PGCD de toutes les valeurs.
 */
//Calcul PGCD
int GetPGCD(int *vals, int nbvals) {
    if (nbvals == 0) return 0;

    int result = vals[0];
    for (int i = 1; i < nbvals; i++) {
        int a = result;
        int b = vals[i];

        // Algorithme d'Euclide pour trouver le PGCD de a et b
        while (b != 0) {
            int temp = b;
            b = a % b; // On prend le reste de la division
            a = temp;  // On décale
        }
        result = a; // Le dernier reste non nul est le PGCD
    }
    return result;
}

//Extraction Sous-Matrice par Classe
t_matrix SubMatrix(t_matrix matrix, t_part part, int compo_index) {
    t_classe cl = part.classes[compo_index];
    int size = cl.nb_vertex;

    t_matrix sub = CreateZeroMatrix(size, size);

    for (int u = 0; u < size; u++) {
        int original_row = cl.sommets[u] - 1;

        for (int v = 0; v < size; v++) {
            int original_col = cl.sommets[v] - 1;
            sub.data[u][v] = matrix.data[original_row][original_col];
        }
    }
    return sub;
}
/**
 * @brief Calcule la période d'une classe (Défi numéro 1)
 *
 * Explication mathématique :
 * La période d'une classe dans un graphe de Markov est le PGCD des longueurs de tous
 * les cycles qui partent d'un état et y reviennent.
 *
 * Algorithme :
 * 1. On calcule les puissances successives de la matrice d'adjacence (M^1, M^2, ..., M^N).
 * 2. La valeur à la position [i][i] de la matrice M^k indique la probabilité (ou l'existence)
 * d'un chemin partant de i et revenant à i en exactement k étapes.
 * 3. Si M^k[i][i] > 0, alors il existe un cycle de longueur k.
 * 4. On collecte toutes ces longueurs k.
 * 5. La période est le PGCD (Plus Grand Commun Diviseur) de ces longueurs.
 *
 * @param sub_matrix La sous-matrice carrée représentant la classe.
 * @return int La période de la classe (1 signifie apériodique).
 */
//Calcul Periode d'une Sous-Matrice
int GetPeriod(t_matrix sub_matrix) {
    int n = sub_matrix.rows;

    // Tableau pour stocker les longueurs de cycles trouvées (max n longueurs possibles ici)
    int *periods = (int*)malloc(n * sizeof(int));
    int period_count = 0;

    // power_matrix va stocker M^1, puis M^2, puis M^3...
    t_matrix power_matrix = CreateZeroMatrix(n, n);

    // On commence avec M^1 (la sous-matrice elle-même)
    CopyMatrix(power_matrix, sub_matrix);

    // On teste les chemins de longueur 1 à n (n = taille de la classe)
    for (int cpt = 1; cpt <= n; cpt++) {
        int diag_nonzero = 0;

        // Vérification de la diagonale s'il existe un cycle de longueur 'cpt'
        for (int i = 0; i < n; i++) {
            // Si power_matrix[i][i] > 0, on peut revenir en i en 'cpt' étapes
            if (power_matrix.data[i][i] > 0.0001f) {
                diag_nonzero = 1;
                break; // Un seul suffit pour valider cette longueur
            }
        }

        // Si un cycle de longueur 'cpt' existe, on l'ajoute à la liste
        if (diag_nonzero) {
            periods[period_count] = cpt;
            period_count++;
        }

        // Préparation de l'étape suivante : Calcul de M^(cpt+1)
        // M^(cpt+1) = M^(cpt) * M
        t_matrix next_power = MultiplyMatrices(power_matrix, sub_matrix);

        // Mise à jour de power_matrix et libération de l'ancienne mémoire
        FreeMatrix(power_matrix);
        power_matrix = next_power;
    }

    // Calcul final : le PGCD de toutes les longueurs de cycles trouvées
    int pgcd = GetPGCD(periods, period_count);

    // Nettoyage de la mémoire
    free(periods);
    FreeMatrix(power_matrix);

    return pgcd;
}