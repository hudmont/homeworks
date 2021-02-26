#include "elim.h"
#define EPSILON 1e-10

#define SLOW
//#define THREADED

int findMaxInRow(int k, int l, double mat[k][l], int row)
{
  int maxindex = 0;
  double maxval=mat[row][0];
  for (int j = 1; j < l; j++) {
    if (fabs(mat[row][j]) > maxval) {
    maxindex = j;
    maxval=fabs(mat[row][j]);
    }
  }
  return maxindex;
}

typedef double vd __attribute__ ((__aligned__(VLEN*8)));

int findDiagPivot(int k, int l, double mat[k][l])
{
  int maxindex = 0;
  double maxval = fabs(mat[0][0]);
  for (int i = 1; i < l; i++) {
    if (fabs(mat[i][i]) >  maxval) {
      maxindex = i;
      maxval =  fabs(mat[i][i]);
    }
  }
  return maxindex;
}


void mulMatRow(int k, int l, double mat[k][l], int rowWhich,
		      double byHowMany)
{
	int last = l - 1;
	int bound = l / VLEN;
	int rem = l % VLEN;
	
        for (int i = 0; i < bound; i++) {

          *(vd *)(&(mat[rowWhich][i*VLEN])) *=byHowMany;
        }

        for (int i=0; i<rem; i++) {
          mat[rowWhich][last-i] *= byHowMany;
        }
}

void addMatRow(int k, int l, double mat[k][l], int rowToWhich, int rowWhich,
		      double mulFactor)
{
	int last = l - 1;
	int bound = l / VLEN;
	int rem = l % VLEN;

        for (int i = 0; i < bound; i ++) {				
          (*(vd *) (&(mat[rowToWhich][i*VLEN]))) +=
            (*(vd *) (&(mat[rowWhich][i*VLEN]))) * mulFactor;	
        }								
        for(int k=0; k < rem; k++){					
          mat[rowToWhich][last-k] +=				
            mat[rowWhich][last-k] * mulFactor;			
        }
}


void invert(int k, double mat[k][k]) {
  // destructive operation
  
  for(int i=0; i < k; i++)
    {
      int pivotPlace = findDiagPivot(k, k, mat);
      double diagPivot = mat[pivotPlace][pivotPlace];

      mat[pivotPlace][pivotPlace]=1.0; // virtualizing the pivot element
      mulMatRow(k, k, mat,pivotPlace, 1.0/diagPivot); // normalizing pivot row
      #pragma omp parallel for
      for(int row=0; row < k; row++)
        {
          if(row==pivotPlace) {
            continue;
          }
          int rowPivotPlace= findMaxInRow(k, k, mat, row);
          double rowPivot = mat[row][rowPivotPlace];    
          mat[row][rowPivotPlace]=0.0; // virtualizing the pivot element
          addMatRow(k, k, mat, row, pivotPlace, -rowPivot); // substract...

        }
      
    }
}
