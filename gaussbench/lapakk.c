#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#include <math.h>

#include "matrix.h"
#include "elim.h"
//#include "lapacke.h"

/*extern void dgesvd(char *jobu , char *jobvt, int m, int n, double *a, int lda,
  double *s, double *u, int ldu, double *vt, int ldvt, double *work, int lwork, int info);*/

typedef struct testdata {
	Matrix mat;
	Matrix inverse;
	double *copy;
} Data;

Data genMat(int n)
{
	int N2 = n * n;

	double *data = (double *)malloc(N2 * sizeof(double));
	Matrix mat = { n, n, {N2, N2, data} };

	for (int k = 0; k < n; k++) {
		for (int i = k; i < n; i++) {
			*get(mat, k, i) = *get(mat, i, k) = rand();
		}
	}

	double *copy = (double *)malloc(N2 * sizeof(double));
	memcpy(copy, data, sizeof(double) * N2);

	Data result = { mat, genInValidMatrix(), copy };

	return result;
}

int main()
{
	int n;
	Data data;
	srand(time(NULL));
	struct timeval start, end;
	double elapsed;

	for (int k = 2; k < 12; k++) {

		n = pow(2, k);
		printf("%d, ",n);
		fflush(stdout);
		data = genMat(n);
		#ifdef DEBUG
		printMat(data.mat);
		#endif
		gettimeofday(&start, NULL);
		Eliminate(data.mat, &data.inverse);
		gettimeofday(&end, NULL);
		elapsed = (end.tv_sec - start.tv_sec) * 1000.0;
		elapsed += (end.tv_usec - start.tv_usec) / 1000.0;
		printf("%lf \n", elapsed);

		matDel(&data.mat);

		matDel(&data.inverse);

	}
}
