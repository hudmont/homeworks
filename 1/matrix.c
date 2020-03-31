#include "matrix.h"

inline int isValidMat(Matrix mat)
{
	if ((mat.rows * mat.cols > mat.arr.size) || (mat.rows < 0)
	    || (mat.rows < 0) || (!isValidArr(mat.arr))) {
		return 0;
	} else {
		return 1;
	}
}

inline Matrix genInValidMatrix()
{
	Matrix result = { -1, -1, {-1, -1, NULL}
	};
	return result;
}

inline void matDel(Matrix * mat)
{
	mat->cols = -1;
	mat->rows = -1;
	arrDel(&(mat->arr));
}

inline double *get(Matrix mat, int row, int col)
{
	/*if((mat.cols <= col) || (mat.rows <= row)) {
	   printf("Érvénytelen indexek: xmax: %d, x: %d, ymax: %d, y: %d \n", mat.cols, col, mat.rows, row);
	   return NULL;
	   } */
	return mat.arr.data + (mat.cols * row) + col;
}

Matrix readMatFromFile(const char *filename)
{
	FILE *fh = fopen(filename, "r");
	if (!fh) {
		fputs
		    ("Nem sikerült fájlt megnyitni a readMatFromFile függvényben!\n",
		     stderr);
		exit(-1);
	}

	Matrix tmp = { 0, 0, {0, 0, NULL}
	};

	Array result;

	while (result = readArrFromLine(fh), result.filled_to > 0) {

		if (tmp.arr.filled_to % result.filled_to) {
			printf("A %s fájl formátuma hibás.\n",
			       filename);
			// mindent felszabadít és a mátrixot érvénytelenné teszi
			free(result.data);
			matDel(&tmp);
			return tmp;
		}

		Array newArr = arrMerge(tmp.arr, result);

		free(tmp.arr.data);

		tmp.arr = newArr;
		tmp.rows++;
		tmp.cols = result.filled_to;

		free(result.data);

	}

	fclose(fh);
	return tmp;
}

Matrix matMul(Matrix left, Matrix right)
{
	Matrix result;

	if (left.cols != right.rows) {
		return genInValidMatrix();
	}

	result.cols = right.cols;
	result.rows = left.rows;

	result.arr.filled_to = result.arr.size =
	    right.cols * left.rows;
	result.arr.data =
	    (double *)calloc(result.arr.size, sizeof(double));
	if (!result.arr.data) {
		fputs
		    ("Nem sikerült memóriát foglalni az arrMerge függvényben!\n",
		     stderr);
		exit(-1);
	}

	for (int k = 0; k < result.rows; k++) {
		for (int m = 0; m < result.cols; m++) {
			for (int l = 0; l < left.cols; l++) {
				(*get(result, k, m)) +=
				    (*get(left, k, l)) *
				    (*get(right, l, m));
			}
		}
	}
	return result;
}

Matrix transpose(Matrix mat)
{
	Matrix result =
	    { mat.cols, mat.rows, {mat.arr.size, mat.arr.filled_to,
				   malloc(mat.arr.size *
					  sizeof(double))}
	};

	for (int i = 0; i < mat.rows; i++) {
		for (int j = 0; j < mat.cols; j++) {
			*get(result, j, i) = *get(mat, i, j);
		}
	}
	return result;
}

void printMat(Matrix mat)
{
	putchar('\n');
	for (int i = 0; i < mat.rows; i++) {
		for (int j = 0; j < mat.cols; j++) {
			printf("%lf ", *(get(mat, i, j)));
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
