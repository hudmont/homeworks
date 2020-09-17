#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#include <math.h>

#include "matrix.h"
#include "elim.h"

#include <omp.h>
//#include "lapacke.h"

/*extern void dgesvd(char *jobu , char *jobvt, int m, int n, double *a, int lda,
  double *s, double *u, int ldu, double *vt, int ldvt, double *work, int lwork, int info);*/

int main()
{
  srand(time(NULL));
  struct timeval start, end;
  double elapsed;
  omp_set_num_threads(16);
  for (int k = 2; k < 12; k++) {

    int n = pow(2, k);
    printf("%d, ",n);
    fflush(stdout);


    double (*mat)[n] = malloc(sizeof(*mat) * n);

    for (int k = 0; k < n; k++) {
      for (int i = k; i < n; i++) {
        mat[k][i] = mat[i][k] = rand();
      }
    }
 
    gettimeofday(&start, NULL);
    invert(n, mat);
    gettimeofday(&end, NULL);
    elapsed = (end.tv_sec - start.tv_sec) * 1000.0;
    elapsed += (end.tv_usec - start.tv_usec) / 1000.0;
                
    printf("%lf \n", elapsed);
                

#ifdef DEBUG
    printMat(n, n, mat);
#endif
    free(mat);

  }
}
