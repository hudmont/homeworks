#include "matrix.h"
#include "elim.h"
//#define K 3

typedef struct f {
	int len;
	int **powers;
} F;

F genPowers(int maxpow, int vars)
{

	int length = (int)pow((double)(maxpow + 1), (double)(vars));
	//printf("eddig: %d\n",length);
	int **pows = (int **)malloc(sizeof(int *) * length);
	int realLen = length;

	for (int i = 0, sum = 0; i < (length); i++, sum = 0) {

		pows[i] = (int *)calloc(sizeof(int), vars);
		//printf("J: %d ", i);
		for (int j = i, k = 0; j > 0; j /= (maxpow + 1), k++) {
			sum += pows[i][k] = j % (maxpow + 1);

		}

		/*for(int k=vars-1; k >= 0; k--)  {
		   printf("%d ", pows[i][k]);
		   } */

		if (sum > (maxpow)) {
			//printf("DEGLETT: %d", i);
			free(pows[i]);
			pows[i] = NULL;
			realLen--;
		}
		//putchar('\n');
	}
	//printf("Valódi hossz: %d \n", realLen);
	int **result = (int **)malloc(sizeof(int *) * realLen);

	for (int i = 0, j = 0; j < realLen; i++) {
		if (pows[i] != NULL) {
			result[j++] = pows[i];
		}
	}
	free(pows);

	F returnVal = { realLen, result };

	return returnVal;
}

F genPowers2(int maxpow, int vars)
{

	int length = (maxpow) * (vars) + 1;
	//printf("eddig: %d\n",length);
	int **pows = (int **)malloc(sizeof(int *) * length);
	pows[0] = (int *)calloc(sizeof(int), vars);

	/*for(int k=vars-1; k >= 0; k--)  {
	   printf("%d ", pows[0][k]);
	   }
	   putchar('\n'); */

	for (int k = 0; k < vars; k++) {
		for (int i = 1; i < (maxpow + 1); i++) {

			pows[i] = (int *)calloc(sizeof(int), vars);

			pows[i][k] = i;

			/*for(int k=vars-1; k >= 0; k--)  {
			   printf("%d ", pows[i][k]);
			   }
			   putchar('\n'); */

		}
	}
	F result = { length, pows };
	return result;

}

int main(int argc, char *argv[])
{
	int K = atoi(argv[2]);
	Matrix data = readMatFromFile(argv[3]);

	int N = data.cols - 2;
	//printf("változók száma: %d\n", N);
	int measurements = data.rows;

	F pows = genPowers(K, N);
	//puts("debug1\n");
	Matrix design =
	    { measurements, pows.len, {measurements * pows.len,
				       measurements * pows.len,
				       (double *)
				       malloc(sizeof(double) *
					      measurements *
					      pows.len)}
	};

	for (int i = 0; i < measurements * pows.len; i++) {
		design.arr.data[i] = 1.0;
	}
	//puts("DEBUG2\n");
	//printf("%d \n", pows.len);
	for (int i = 0; i < measurements; i++) {
		for (int powindex = 0; powindex < pows.len;
		     powindex++) {
			for (int varindex = 0; varindex < N;
			     varindex++) {
				//printf("Random debug üzi: i:%d powindex:%d varindex:%d",i,powindex,varindex);
				*get(design, i, powindex) *= (pow(*get(data, i, varindex), pows.powers[powindex][varindex]))	/*(*get(data,i,N+1)) */
				    ;
			}
		}
	}
	free(pows.powers);
	//printf("OKS\n");
	Matrix b = { measurements, 1, {measurements, measurements,
				       malloc(sizeof(double) *
					      measurements)}
	};

	for (int i = 0; i < measurements; i++) {
		*get(b, i, 0) =
		    *get(data, i, N) /*(*get(data,i,N+1)) */;
	}
	//printMat(b);
	//printMat(data);
	//printMat(design);

	Matrix tr = transpose(design);

	Matrix right2 = matMul(tr, b);

	Matrix left = matMul(tr, design);
	//printMat(right2);
	//putchar('\n');
	matDel(&tr);
	//printMat(left);
	Matrix inverse = genInValidMatrix();
	Eliminate(left, &inverse);
	Matrix right = matMul(inverse, right2);

	puts("Kovarianciamátrix:\n");
	printMat(inverse);
	puts("Illesztési együtthatók:\n");
	printMat(right);

	Matrix result = matMul(design, right);

	matDel(&inverse);
	matDel(&right);
	matDel(&right2);
	matDel(&design);
	double chi2 = 0;
	double y = 0;
	FILE *fh = fopen(argv[4], "w");
	for (int i = 0; i < data.rows; y = 0, i++) {

		y += *get(result, i,
			  0) /**pow(*get(data,i,N+1),2.0)*/ ;

		fprintf(fh, "%f %f\n", *get(data, i, N), y);
		//printf("adalék: %f\n", pow(*get(data,i,N)-y,2.0));
		chi2 += pow(*get(data, i, N) - y, 2.0);
	}
	fclose(fh);

	double ndf = (double)(measurements - pows.len);

	matDel(&data);
	matDel(&result);

	printf("χ²: %f\nχ²/NDF: %f \n", ndf, chi2, chi2 / ndf);
}
