#ifndef ARRAY_H
#define ARRAY_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

typedef struct array {
	int size;
	int filled_to;
	double *data;
} Array;

void arrDel(Array * arr);

Array arrMerge(Array arr1, Array arr2);

void appendToArr(Array * arr, double val);

Array readArrFromLine(FILE * fh);

int isValidArr(Array arr);

void printArr(Array arr);

#endif
