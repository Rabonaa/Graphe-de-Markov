#ifndef INC_2526_TI301_PJT_MATRIX_H
#define INC_2526_TI301_PJT_MATRIX_H

#ifndef __MATRIX_H__
#define __MATRIX_H__

#include "utils.h"

typedef struct {
    float **data;
    int rows;
    int cols;
} t_matrix;

t_matrix CreateZeroMatrix(int rows, int cols);
t_matrix AdjacencyToMatrix(t_liste_adj list_adj);
void CopyMatrix(t_matrix dest, t_matrix src);
void DisplayMatrix(t_matrix mat);
t_matrix MultiplyMatrices(t_matrix A, t_matrix B);
float DiffMatrix(t_matrix A, t_matrix B);
void FreeMatrix(t_matrix mat);
t_matrix SubMatrix(t_matrix matrix, t_part part, int compo_index);
int GetPGCD(int *vals, int nbvals);
int GetPeriod(t_matrix sub_matrix);

#endif

#endif //INC_2526_TI301_PJT_MATRIX_H