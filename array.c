#include "array.h"

inline void arrDel(Array * arr)
{
	arr->size = -1;
	arr->filled_to = -1;
	free(arr->data);
	arr->data = NULL;
}

inline int isValidArr(Array arr)
{
	return ((arr.size >= 0) && (arr.filled_to >= 0)
		&& (arr.filled_to <= arr.size) && (arr.data != NULL));
}

Array arrMerge(Array arr1, Array arr2)
{
	// mindig újrafoglal szóval még lehet gyorsítani rajta
	Array result;
	result.size = arr1.size + arr2.size;

	result.filled_to = arr1.filled_to + arr2.filled_to;

	result.data =
	    (double *)malloc((arr1.size + arr2.size) *
			     sizeof(double));
	if (!result.data) {
		fputs
		    ("Nem sikerült memóriát foglalni az arrMerge függvényben!\n",
		     stderr);
		exit(-1);
	}

	memcpy(result.data, arr1.data,
	       sizeof(double) * arr1.filled_to);
	memcpy(result.data + arr1.filled_to, arr2.data,
	       sizeof(double) * arr2.filled_to);
	return result;
}

void appendToArr(Array * arr, double val)
{
	assert(arr->filled_to <= arr->size);

	if (arr->filled_to == arr->size) {
		double *newArr =
		    (double *)malloc(arr->size * 2 * sizeof(double));
		if (!newArr) {
			fputs
			    ("Nem sikerült memóriát foglalni az appendToArr függvényben!\n",
			     stderr);
			exit(-1);
		}
		memcpy(newArr, arr->data,
		       sizeof(double) * arr->filled_to);

		free(arr->data);
		arr->data = newArr;
		arr->size *= 2;
	}

	arr->data[arr->filled_to] = val;
	arr->filled_to++;
}

Array readArrFromLine(FILE * fh)
{
	Array tmp = { 4, 0, (double *)malloc(4 * sizeof(double)) };
	if (!tmp.data) {
		fputs
		    ("Nem sikerült memóriát foglalni a readArrFromLine függvényben!\n",
		     stderr);
		exit(-1);
	}

	int i = 0;
	char c;
	double value;
	do {
		i = fscanf(fh, "%lf%c", &value, &c);
		if ((i == EOF) && (tmp.filled_to == 0)) {
			arrDel(&tmp);
			return tmp;
		}
		appendToArr(&tmp, value);
	} while ((i != EOF) && c != '\n');

	// fájl végén van ez, ekkor érvénytelen vektort ad vissza
	/*printf("%d %lf,Aktualis karakter:aa%caa, A tömb:",i,value,c); */
	/*for(int j=0; j<tmp.filled_to; j++) {
	   printf("%lf ", tmp.data[j]);
	   }
	   putchar('\n'); */

	fflush(fh);
	while ((c != '\n') && (c != EOF)) {
		c = fgetc(fh);
		//printf("aa%cbb \n",c);
		//tmp.filled_to--;
	}

	//printf("%d aa%caa \n", i,c);    

	// printArr(tmp);
	return tmp;

}

void printArr(Array arr)
{
	putchar('\n');
	for (int i = 0; i < arr.filled_to; i++) {
		printf("%lf ", arr.data[i]);
	}
	putchar('\n');
}
