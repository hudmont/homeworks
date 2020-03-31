#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
//#include <cblas.h>
#include <lapacke.h>
#include "lapacke.h"
//#include "lapacke_utils.h"
//#include "lapacke_example_aux.h"

//sudo gcc --std=c99 lapsvd.c -o LAP -llapacke -llapack -lblas -lm -Wall

double *read(FILE * file, double *m, int matrixsiz[2]);
double *adddouble(double D, int MATRNUM, double *M);
double *copyelements(int L, double *FROM, double *TO);

int main(int arg, const char *argv[])
{
	int matrixsize[2];
	matrixsize[0] = matrixsize[1] = 0;
	double *A;
	A = (double *)malloc(0 * (sizeof(double)));

	FILE *matr;
	matr = fopen(argv[1], "r");
	A = read(matr, A, matrixsize);
	fclose(matr);
	printf("\nA:\n");
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			printf("%lf ", A[i * 3 + j]);
		}
		printf("\n");
	}
	printf("\n");

	printf("\n%ix%i\n", matrixsize[0], matrixsize[1]);

	double *S;
	S = (double *)malloc((matrixsize[0] * matrixsize[1]) *
			     (sizeof(double)));
	double *U;
	U = (double *)malloc((matrixsize[0] * matrixsize[1]) *
			     (sizeof(double)));
	double *VT;
	VT = (double *)malloc((matrixsize[0] * matrixsize[1]) *
			      (sizeof(double)));
	double *SUPERB;
	SUPERB = (double *)malloc(matrixsize[0] * (sizeof(double)));
	lapack_int N = matrixsize[0];

	lapack_int info =
	    LAPACKE_dgesvd(LAPACK_ROW_MAJOR, 'A', 'A', N, N, A, N, S,
			   U, N, VT,
			   N, SUPERB);
	printf("\n%i\n\n", info);

	printf("\nU:\n");
	for (int i = 0; i < matrixsize[0]; i++) {
		for (int j = 0; j < matrixsize[0]; j++) {
			printf("%lf ", U[i * matrixsize[0] + j]);
		}
		printf("\n");
	}
	printf("\n");

	printf("\nS:\n");
	for (int i = 0; i < matrixsize[0]; i++) {
		for (int j = 0; j < matrixsize[0]; j++) {
			printf("%lf ", S[i * matrixsize[0] + j]);
		}
		printf("\n");
	}
	printf("\n");

	printf("\nVT:\n");
	for (int i = 0; i < matrixsize[0]; i++) {
		for (int j = 0; j < matrixsize[0]; j++) {
			printf("%lf ", VT[i * matrixsize[0] + j]);
		}
		printf("\n");
	}
	printf("\n");

	//A=U*S*VT (?)
	//Aij=UikSklVTjl
	//Aij=UikBkj
	//Bkj=sum(l)SklVTlj

	for (int i = 0; i < matrixsize[0]; i++) {
		for (int j = 0; j < matrixsize[0]; j++) {
			A[i * 3 + j] = ((double)(0.0));
			for (int l = 0; l < matrixsize[0]; l++) {
				A[i * 3 + j] += ((double)
						 (((double)
						   (U[i * 3 + l])) *
						  ((double)(S[l])) *
						  ((double)
						   (VT[3 * l + j]))));
			}
		}
	}

	printf("\nA:\n");
	for (int i = 0; i < matrixsize[0]; i++) {
		for (int j = 0; j < matrixsize[0]; j++) {
			printf("%lf ", A[i * matrixsize[0] + j]);
		}
		printf("\n");
	}
	printf("\n");

	for (int i = 0; i < matrixsize[0]; i++) {
		for (int j = 0; j < matrixsize[0]; j++) {
			A[i * 3 + j] = ((double)(0.0));
			for (int l = 0; l < matrixsize[0]; l++) {
				A[i * 3 + j] += ((double)
						 (((double)
						   (VT[l * 3 + i])) *
						  ((double)(1 / S[l]))
						  *
						  ((double)
						   (U[3 * j + l]))));
			}
		}
	}

	printf("\nA:\n");
	for (int i = 0; i < matrixsize[0]; i++) {
		for (int j = 0; j < matrixsize[0]; j++) {
			printf("%lf ", A[i * matrixsize[0] + j]);
		}
		printf("\n");
	}
	printf("\n");

	return 0;

}

double *read(FILE * file, double *m, int matrixsize[2])
{

	int matrnum = 0;
	int ROWS = 0;
	int COLS = 0;
	int i = 0;
	double d;
	char ch;
	while (1) {

		i = fscanf(file, "%lf%c", &d, &ch);
		if ((i <= 0) || (ch == -1) || (ch == EOF)) {

			break;

		}
		m = adddouble(d, matrnum, m);
		matrnum++;
		COLS++;
		if ((ch == '\n') || (i <= 1) || (ch == 13)
		    || (ch == -1)
		    || (ch == EOF)) {

			if (COLS >= matrixsize[1]) {

				matrixsize[1] = COLS;

			}
			COLS = 0;
			ROWS++;

		}

	}
	matrixsize[0] = ROWS;

	return m;

}

double *copyelements(int L, double *FROM, double *TO)
{
	for (int j = 0; j < L; j++) {

		TO[j] = FROM[j];

	}

	return TO;

}

double *adddouble(double D, int MATRNUM, double *M)
{

	MATRNUM = MATRNUM + 1;
	double *MM;
	MM = (double *)malloc(MATRNUM * (sizeof(double)));
	MM = copyelements((MATRNUM - 1), M, MM);
	free(M);
	M = (double *)malloc(MATRNUM * (sizeof(double)));
	M = copyelements((MATRNUM - 1), MM, M);
	M[(MATRNUM - 1)] = D;
	free(MM);

	return M;

}
