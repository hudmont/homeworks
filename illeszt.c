#include "matrix.h"
#include "elim.h"
#define SZIGMA(i) (*get(data,i,N+1))
#define MERT(i)   (*get(data, i, N))

typedef struct f {
	int len;
	int **powers;
} F;



Matrix read(const char *fileName, int vars)
{
    FILE *fh = fopen(fileName, "r");
    
    Matrix result;
    result.cols=vars+2;
    result.arr.data = (double *) malloc((vars+2)*sizeof(double));
    int row=0;
    int ok;

    for(row=0; !feof(fh); row++) {

        result.arr.data = (double *) realloc(result.arr.data,(row+1)*(vars+2)*sizeof(double));
        for(int k=0; k < result.cols; k++) {
            ok=fscanf(fh, "%lf", get(result, row, k));
        }
        if(!ok) {
            result.arr.data = (double *) realloc(result.arr.data,(row)*(vars+2)*sizeof(double));
            fputs("Hiba történt fájlbeolvasánál! Fájlnév:",stderr);
            fputs(fileName,stderr);
            row--;
            break;
        }
    }
    
    
    result.rows = row-1;
    result.arr.filled_to = result.arr.size = result.rows*result.cols;
    
    fclose(fh);
    return result;
}


F genPowers(int maxpow, int vars)
{

	int length = (int)pow((double)(maxpow + 1), (double)(vars));

	int **pows = (int **)malloc(sizeof(int *) * length);
	int realLen = length;

	for (int i = 0, sum = 0; i < (length); i++, sum = 0) {

		pows[i] = (int *)calloc(sizeof(int), vars);

		for (int j = i, k = 0; j > 0; j /= (maxpow + 1), k++) {
			sum += pows[i][k] = j % (maxpow + 1);

		}

		if (sum > (maxpow)) {
			free(pows[i]);
			pows[i] = NULL;
			realLen--;
		}
	}
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

int main(int argc, char *argv[])
{
    if(argc != 5) {
        fputs("Hibás programmeghívás!",stderr);
    }
    int N = atoi(argv[1]);
	int K = atoi(argv[2]);
	Matrix data = read(argv[3],N);
	int measurements = data.rows;

	F pows = genPowers(K, N);

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
    //double max=0;;
	for (int i = 0; i < measurements; i++) {
		for (int powindex = 0; powindex < pows.len;
		     powindex++) {
			for (int varindex = 0; varindex < N;
			     varindex++) {

				*get(design, i, powindex) *= (pow(*get(data, i, varindex), pows.powers[powindex][varindex])); ;
			}
			//*get(design, i, powindex) /*= SZIGMA(i)*/;
            /*if(fabs(*get(design, i, powindex)) > max) {
                max = fabs(*get(design, i, powindex));
            }*/
		}
	}
	free(pows.powers);
    /*// gyanús hogy normálni kéne...
    for (int i = 0; i < measurements; i++) {
		for (int powindex = 0; powindex < pows.len;
		     powindex++) {
                 *get(design, i, powindex) /= max;
        }
    }*/
    
	Matrix b = { measurements, 1, {measurements, measurements,
				       malloc(sizeof(double) *
					      measurements)}
	};

	for (int i = 0; i < measurements; i++) {
		*get(b, i, 0) =
		    MERT(i) /* SZIGMA(i) /* max*/;
	}


	Matrix tr = transpose(design);

	Matrix right2 = matMul(tr, b);

	Matrix left = matMul(tr, design);

	matDel(&tr);

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
	double y;
	FILE *fh = fopen(argv[4], "w");
	for (int i = 0; i < data.rows; i++) {

		y = *get(result, i, 0);

		fprintf(fh, "%f %f\n", MERT(i), y);

		chi2 += pow(MERT(i) - y, 2.0)/*pow(SZIGMA(i),2.0)*/;
	}
	fclose(fh);

	double ndf = (double)(measurements - pows.len);

	matDel(&data);
	matDel(&result);

	printf("χ²: %f\nχ²/NDF: %f \n", chi2, chi2 / ndf);
}
