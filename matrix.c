#include "matrix.h"

/* inline int isValidMat(Matrix mat) */
/* { */
/* 	if ((mat.rows * mat.cols > mat.arr.size) || (mat.rows < 0) */
/* 	    || (mat.rows < 0) || (!isValidArr(mat.arr))) { */
/* 		return 0; */
/* 	} else { */
/* 		return 1; */
/* 	} */
/* } */

/* inline Matrix genInValidMatrix() */
/* { */
/*   Matrix result = { -1, -1, {-1, -1, NULL}}; */
/*   return result; */
/* } */

/* inline void matDel(Matrix * mat) */
/* { */
/*   mat->cols = -1; */
/*   mat->rows = -1; */
/*   arrDel(&(mat->arr)); */
/* } */

/* inline double *get(Matrix mat, int row, int col) */
/* { */
/* 	return mat.arr.data + (mat.cols * row) + col; */
/* } */

/* Matrix readMatFromFile(const char *filename) */
/* { */
/* 	FILE *fh = fopen(filename, "r"); */
/* 	if (!fh) { */
/* 		fputs */
/* 		    ("Nem sikerült fájlt megnyitni a readMatFromFile függvényben!\n", */
/* 		     stderr); */
/* 		exit(-1); */
/* 	} */

/* 	Matrix tmp = { 0, 0, {0, 0, NULL} }; */

/* 	Array result; */

/* 	while (result = readArrFromLine(fh), result.filled_to > 0) { */

/* 		if (tmp.arr.filled_to % result.filled_to) { */
/* 			printf("A %s fájl formátuma hibás.\n", */
/* 			       filename); */
/* 			// mindent felszabadít és a mátrixot érvénytelenné teszi */
/* 			free(result.data); */
/* 			matDel(&tmp); */
/* 			return tmp; */
/* 		} */

/* 		Array newArr = arrMerge(tmp.arr, result); */

/* 		free(tmp.arr.data); */

/* 		tmp.arr = newArr; */
/* 		tmp.rows++; */
/* 		tmp.cols = result.filled_to; */

/* 		free(result.data); */

/* 	} */

/* 	fclose(fh); */
/* 	return tmp; */
/* } */

double *matMul(int k, int l, double left[k][l], int m,  double right[l][m])
{
  double (*result)[m] = malloc(sizeof(*result) * k);;

  if (!result) {
    fputs("Nem sikerült memóriát foglalni az arrMerge függvényben!\n",
       stderr);
    exit(-1);
  }

  for (int p = 0; p < k; p++) {
    for (int q = 0; q < m; q++) {
      for (int r = 0; r < l; r++) {
        result[p][q] += left[p][r] * right[r][q];
      }
    }
  }
  return (double *)result;
}

double *transpose(int k, int l, double mat[k][l])
{
  
  double (*result)[l] = malloc(sizeof(*result) * k);
  
  for (int i = 0; i < k; i++) {
    for (int j = 0; j < l; j++) {
      result[j][i] = mat[i][j];
    }
  }
  return (double *)result;
}

void printMat(int k, int l, double mat[k][l])
{
  putchar('\n');
  for (int i = 0; i < l; i++) {
    for (int j = 0; j < l; j++) {
      printf("%lf ", mat[i][j]);
    }
    putchar('\n');
  }
  putchar('\n');
}

#ifdef UNITTEST
int main(int argc, char *argv[])
{

  Matrix mat1 = readMatFromFile("testmatrix1.txt");

  printMat(mat1);
  //printArr(mat1.arr);

  Matrix mat4 = readMatFromFile("testmatrix4.txt");

  printMat(mat4);
  Matrix product = matMul(mat4, mat1);

  printMat(product);

  matDel(&product);
  matDel(&mat1);
  matDel(&mat4);

  Matrix result2 = readMatFromFile("testmatrix2.txt");

  matDel(&result2);

  Matrix result3 = readMatFromFile("testmatrix3.txt");
  printMat(result3);

  Matrix mat5 = readMatFromFile("testmatrix5.txt");
  printMat(mat5);

  product = matMul(mat5, result3);
  printMat(product);
  matDel(&product);

  matDel(&result3);
  return 0;
}
#endif
