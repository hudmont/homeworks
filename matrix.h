#ifndef MATRIX_H
#define MATRIX_H

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "array.h"

typedef struct matrix {
	int rows;
	int cols;
	Array arr;
} Matrix;

//void matDel(Matrix * mat);

//double *get(Matrix mat, int row, int col);

//void checkMat(Matrix mat);

//Matrix readMatFromFile(const char *filename);

//Matrix genInValidMatrix(void);

//int isValidMat(Matrix mat);

double *matMul(int k, int l, double left[k][l], int m, double right[l][m]);

void printMat(int k, int l, double mat[k][l]);

double *transpose(int k, int l, double mat[k][l]);

#endif
