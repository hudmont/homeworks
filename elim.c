#include "elim.h"
#define EPSILON 1e-10

#define SLOW
//#define THREADED

int findMaxInRow(Matrix mat, int row)
{
	int maxindex = 0;
	for (int k = 0; k < mat.cols; k++) {
		if (fabs(*get(mat, row, k)) >
		    fabs(*get(mat, row, maxindex))) {
			maxindex = k;
		}
	}
	return maxindex;
}

int findMaxInCol(Matrix mat, int col, int startingWith)
{
	int maxindex = startingWith;
	for (int k = startingWith; k < mat.rows; k++) {
		if (fabs(*get(mat, k, col)) >
		    fabs(*get(mat, maxindex, col))) {
			maxindex = k;
		}
	}
	return maxindex;
}

XY findMax(Matrix mat, int underRow, int afterCol)
{
	XY max = { underRow, afterCol };
	for (int k = underRow; k < mat.rows; k++) {
		for (int l = afterCol; l < mat.cols; l++) {
			if (fabs(*get(mat, k, l)) >
			    fabs(*get(mat, max.x, max.y))) {
				max.x = k;
				max.y = l;
			}
		}
	}
	return max;
}

void swapRow(Eliminator * elim, int rowWhich, int rowWithWhich)
{

	if (rowWhich == rowWithWhich) {
		//printf("debug: sort önmagára cserélt");
		return;
	}

	double *temp =
	    malloc(elim->left->cols * sizeof(double));

	memcpy(temp, get(*elim->left, rowWhich, 0),
	       elim->left->cols * sizeof(double));

	memcpy(get(*elim->left, rowWhich, 0),
	       get(*elim->left, rowWithWhich, 0),
	       elim->left->cols * sizeof(double));

	memcpy(get(*elim->left, rowWithWhich, 0), temp,
	       elim->left->cols * sizeof(double));

	free(temp);
        
	temp = malloc(elim->right->cols * sizeof(double));

	memcpy(temp, get(*elim->right, rowWhich, 0),
	       elim->right->cols * sizeof(double));
	memcpy(get(*elim->right, rowWhich, 0),
	       get(*elim->right, rowWithWhich, 0),
	       elim->right->cols * sizeof(double));
	memcpy(get(*elim->right, rowWithWhich, 0), temp,
	       elim->right->cols * sizeof(double));

	free(temp);
}

void swapCol(Eliminator * elim, int colWhich, int colWithWhich)
{
	if (colWhich == colWithWhich) {
		//printf("debug: oszlopot önmagára cserélt");
		return;
	}
	double *temp =
	    malloc(elim->left->rows * sizeof(double));

	// feltételezzük, hogy ugyanannyi soros mindkét oldal

	for (int k = 0; k < elim->left->rows; k++) {
		temp[k] = *get(*elim->left, k, colWhich);
	}

	for (int k = 0; k < elim->left->rows; k++) {
		*get(*elim->left, k, colWhich) =
		    *get(*elim->left, k, colWithWhich);
	}

	for (int k = 0; k < elim->left->rows; k++) {
		*get(*elim->left, k, colWithWhich) = temp[k];
	}

	free(temp);

	int tmp = elim->colswaps[colWhich];
	elim->colswaps[colWhich] = elim->colswaps[colWithWhich];
	elim->colswaps[colWithWhich] = tmp;
}

typedef double vd __attribute__ ((__aligned__(VLEN*8)));

void addRow(Eliminator * elim, int rowToWhich, int rowWhich,
		      double mulFactor)
{
	int last = elim->left->cols - 1;
	int bound = last / VLEN;
	int rem=elim->left->cols % VLEN;
        inline void doOneRow(Matrix mat) {
          for (int k = 0; k < bound; k ++) {				
            (*(vd *) (get(mat, rowToWhich, k*VLEN))) +=
              (*(vd *) (get(mat, rowWhich,   k*VLEN))) * mulFactor;	
          }								
          for(int k=0; k < rem; k++){					
            *get(mat, rowToWhich, last-k) +=				
              *get(mat, rowWhich,   last-k) * mulFactor;			
          }
        }
	doOneRow(*elim->left);
	doOneRow(*elim->right);
}


void mulRow(Eliminator * elim, int rowWhich,
		      double byHowMany)
{
	int last = elim->left->cols - 1;
	int bound = elim->left->cols / VLEN;
	int rem = elim->left->cols % VLEN;
	
	inline void doOneRow(Matrix mat) {
	  for (int k = 0; k < bound; k++) {				
	    *(vd *)(get(mat, rowWhich, k*VLEN)) *=byHowMany;
	  }								
									
	  for (int k=0; k<rem; k++) {					
	    *get(mat, rowWhich, last-k) *=				
	      byHowMany;						
	  }								
        }

	doOneRow(*elim->left);
	doOneRow(*elim->right);
}
void Eliminate(Matrix mat, Matrix * right)
{

	Matrix left = mat;

	Eliminator elim;

#ifndef NOCOPY
	left.arr.data =
	    (double *)malloc(mat.arr.filled_to * sizeof(double));
	memcpy(left.arr.data, mat.arr.data,
	       mat.arr.filled_to * sizeof(double));
#endif

	elim.left = &left;

	int *colswaps = (int *)malloc(mat.cols * sizeof(int));
	for (int k = 0; k < mat.cols; k++) {
		colswaps[k] = k;
	}

	elim.colswaps = colswaps;

	if (!isValidMat(*right)) {
		right->rows = mat.rows;
		right->cols = mat.cols;
		right->arr.size = mat.rows * mat.cols;
		right->arr.filled_to = mat.rows * mat.cols;
		right->arr.data =
		    (double *)calloc(mat.rows * mat.cols,
				     sizeof(double));

		for (int k = 0; k < mat.cols; k++) {
			*get(*right, k, k) = 1.0;
		}

	}

	elim.right = right;

	// normálás
	double pivot;

	XY pivotcoords;

	double leadingElement;
	int rows = elim.left->rows;
	for (int i = 0; i < rows; i++) {

#ifdef THREADED
#pragma omp parallel for
#endif
		for (int k = i; k < elim.left->cols; k++) {
			pivot =
			    *get(*elim.left, k,
				 findMaxInRow(*elim.left, k));

			mulRow(&elim, k, 1.0 / pivot);
#ifdef DEBUG
			printf("Anjád: %d\n",i);
#endif
		}
#ifdef DEBUG
		puts("b4 backelim\n");
#endif
#ifdef PRECISE
		pivotcoords = findMax(*elim.left, i, i);

#else
		pivotcoords.x = findMaxInCol(*elim.left, i, i);
		pivotcoords.y = i;
#endif
#ifdef DEBUG
		puts("b4 backelim\n");
#endif
		pivot =
		    *get(*elim.left, pivotcoords.x, pivotcoords.y);
#ifdef DEBUG
		puts("b4 backelim\n");
#endif
		if (fabs(pivot) < EPSILON) {

			matDel(elim.right);
#ifndef NOCOPY
			matDel(elim.left);
#endif
			(*right) = (*elim.right);
			return;
		}

		swapRow(&elim, i, pivotcoords.x);

#ifdef PRECISE
		swapCol(&elim, i, pivotcoords.y);
#endif
#ifdef THREADED
#pragma omp parallel for
#endif
		for (int j = i + 1; j < rows; j++) {
			leadingElement = *get(*elim.left, j, i);
			addRow(&elim, j, i, -leadingElement / pivot);

		}

	}

	for (int i = rows - 1; i >= 0; i--) {
		// Most minden normálva van elvileg

#ifdef THREADED
#pragma omp parallel for
#endif
		for (int j = i - 1; j >= 0; j--) {
			addRow(&elim, j, i,
			       -*get(*elim.left, j,
				     i) / *get(*elim.left, i, i));
		}

		// kihozza 1-re az aktuális sort

		mulRow(&elim, i, 1.0 / *get(*elim.left, i, i));


		// normálja a felsőbb sorokat
#ifdef THREADED
#pragma omp parallel for
#endif
		for (int k = i - 1; k >= 0; k--) {
			pivot =
			    *get(*elim.left, k,
				 findMaxInRow(*elim.left, k));

			mulRow(&elim, k, 1.0 / pivot);

		}

	}

	// változók visszacseréje, majd diagonálissá alakítás ismét
#ifdef PRECISE
	for (int k = 0; k < elim.left->cols; k++) {
		if (elim.colswaps[k] != k) {
			for (int i = k + 1; i < elim.left->cols; i++) {
				if (elim.colswaps[i] == k) {
					swapCol(&elim, k, i);
					swapRow(&elim, k, i);
				}
			}
		}
	}
#endif

#ifdef SLOW
	matDel(elim.left);
#endif
	free(colswaps);

	(*right) = (*elim.right);
}
