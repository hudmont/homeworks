#ifndef ELIM_H
#define ELIM_H

#include "matrix.h"
#include <math.h>

typedef struct eliminator {
	Matrix *left;
	Matrix *right;
	int *colswaps;
} Eliminator;

typedef struct xy {
	int x, y;
} XY;

int findMaxInRow(Matrix mat, int row);
int findMaxInCol(Matrix mat, int col, int startingWith);
void swapRow(Eliminator * elim, int rowWhich, int rowWithWhich);
void addRow(Eliminator * elim, int rowToWhich, int rowWhich,
	    double mulFactor);
void mulRow(Eliminator * elim, int rowWhich, double byHowMany);
void Eliminate(Matrix mat, Matrix * right);

#endif
