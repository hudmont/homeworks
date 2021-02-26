#include "elim.h"
#include "matrix.h"

int main(int argc, char *argv[])
{
	if(argc ==1) {
		exit(-1);
	}
	/*double pointer[16] =    {   3, 2,  0,  0,
	   4, 3,  0,  0,
	   0, 0,  6,  5,
	   0, 0,  7,  6};

	   double copied[16] =    {   3, 2,  0,  0,
	   4, 3,  0,  0,
	   0, 0,  6,  5,
	   0, 0,  7,  6};
	   int rows,cols;
	   rows=cols=4;

	   Matrix mat={rows,cols,{16,16,(double *)pointer}};
	   Matrix replica={rows,cols,{16,16,(double *)copied}}; */

	Matrix mat = readMatFromFile(argv[1]);
	Matrix replica = mat;
	replica.arr.data = malloc(sizeof(double) * replica.arr.size);
	memcpy(replica.arr.data, mat.arr.data,
	       sizeof(double) * replica.arr.size);

	printMat(mat);
	putchar('\n');

	Matrix inverse = genInValidMatrix();

	Eliminate(mat, &inverse);
	if (!isValidMat(inverse)) {
		printf("A mátrix nem invertálható.");
	}

	printMat(inverse);
	putchar('\n');

	Matrix result = matMul(replica, inverse);
	matDel(&replica);
	printMat(result);

	matDel(&result);
	matDel(&inverse);

	return 0;
}
