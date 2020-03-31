#include <stdio.h>
//#include <matrix.h>
#include <math.h>
#include <stdlib.h>

//
inline double max(double a, double b)
{
	return (a > b) ? a : b;
}
inline double squaresum(double a, double b)
{
    return a*a+b*b;
}
typedef double *Vector;

typedef struct state {
	Vector r;
	Vector v;
	double t;
} State;

static double h = 1.0;
static double D = 1.0;

const double GAMMAM = 398668.50624;
static double Dmax;

void printState(State s, FILE *where)
{
    fprintf(where, "%f ", s.t);
	fprintf(where, "%f ", s.r[0]);
	fprintf(where, "%f ", s.r[1]);
	fprintf(where, "%f ", s.v[0]);
	fprintf(where, "%f ", s.v[1]);
	fprintf(where, "%f\n", squaresum(s.v[0],s.v[1])+GAMMAM/pow(squaresum(s.r[0],s.r[1]), 0.5));
}

State apply(State current, State differential, double multiplier)
{
	State result;

	result.r = (double *)malloc(sizeof(double) * 2);
	result.v = (double *)malloc(sizeof(double) * 2);

	result.r[0] = current.r[0] + multiplier * differential.r[0];
	result.r[1] = current.r[1] + multiplier * differential.r[1];

	result.v[0] = current.v[0] + multiplier * differential.v[0];
	result.v[1] = current.v[1] + multiplier * differential.v[1];

	result.t = current.t + multiplier;

	return result;
}

State moonsystem(State current)
{
	State result;
    
	result.r = (double *)malloc(sizeof(double) * 2);
	result.v = (double *)malloc(sizeof(double) * 2);
	const double factor =
	    GAMMAM * pow(squaresum(current.r[0], current.r[1]), -1.5);

	result.r[0] = current.v[0];
	result.r[1] = current.v[1];
	result.v[0] = -current.r[0] * (factor);
	result.v[1] = -current.r[1] * (factor);


	return result;
}

State RK4(State current, State(*genDifferentials) (State))
{

	State k_1 = (*genDifferentials) (current);

	State intermediate = apply(current, k_1, 0.5 * h);

	State k_2 = (*genDifferentials) (intermediate);

	State intermediate2nd = apply(current, k_2, 0.5 * h);

	State k_3 = (*genDifferentials) (intermediate2nd);

	State intermediate3rd = apply(current, k_3, 1.0 * h);

	State k_4 = (*genDifferentials) (intermediate3rd);


	State result1 = apply(current, k_1, 0.1666666667 * h);
    State result2 = apply(result1, k_2, 0.3333333333 * h);
    State result3 = apply(result2, k_3, 0.3333333333 * h);
    State result4 = apply(result3, k_4, 0.1666666667 * h);

    double c = max(max(current.r[0]-result4.r[0],current.r[1]-result4.r[1]), max(current.v[0]-result4.v[0],current.v[1]-result4.v[1]));
    if(c > D){
        h *= (pow(fabs(Dmax / D), 0.2));
    }
    
    free(k_1.r);
	free(k_2.r);
	free(k_3.r);
	free(k_4.r);
	free(k_1.v);
	free(k_2.v);
	free(k_3.v);
	free(k_4.v);

	free(intermediate.r);
	free(intermediate.v);
	free(intermediate2nd.r);
	free(intermediate2nd.v);
	free(intermediate3rd.r);
	free(intermediate3rd.v);
    
    free(result1.r);
    free(result2.r);
    free(result3.r);
    free(result1.v);
    free(result2.v);
    free(result3.v);

	return result4;
}

int main(void)
{
    Dmax = 1.0;
    //atof(argv[argc-1]);
	State startingConditions;

	startingConditions.r = (double *)malloc(sizeof(double) * 2);
	startingConditions.v = (double *)malloc(sizeof(double) * 2);
	startingConditions.r[0] = 363300.0;
	startingConditions.r[1] = 0.0;
	startingConditions.v[0] = 0.0;
	startingConditions.v[1] = 1.076;
    startingConditions.t=0.0;

	State current = startingConditions;
	State next;

	for (int i = 0; current.t < 3548800; i++) {

		printState(current,stdout);
		next = RK4(current, &moonsystem);

		free(current.r);
		free(current.v);
		current = next;

	}
	free(next.r);
	free(next.v);
}
