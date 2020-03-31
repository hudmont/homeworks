#include <stdio.h>
//#include <matrix.h>
#include <math.h>
#include <stdlib.h>

//
/*inline*/ double max(double a, double b)
{
	return a > b ? a : b;
}

typedef double *Vector;

typedef struct state {
	Vector r;
	Vector v;
	double t;
} State;

static double h = 0.01;
const static double Dmax = 1.075;
static double D = 1.0;

const double GAMMA = 6.67384e-20;
const double M = 5.9736e24;
const double GAMMAM = 398668.50623999996;

void printState(State s)
{
	printf("x: %f ", s.r[0]);
	printf("y: %f ", s.r[1]);
	printf("vx: %f ", s.v[0]);
	printf("vy: %f ", s.v[1]);
	printf("t: %f\n", s.t);
}

State apply(State current, State differential, double multiplier)
{
	State result;

	// multiplier: 1/2*h, etc.
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
	//printState(current);
	result.r = (double *)malloc(sizeof(double) * 2);
	result.v = (double *)malloc(sizeof(double) * 2);
	const double factor =
	    GAMMAM / pow(current.r[0] * current.r[0] +
			 current.r[1] * current.r[1], 1.5);
	// ne legyen már kerekítési hiba errefele

	// GAMMAM / pow(pow(current.r.x,2.0)+pow(current.r.y,2.0),3.0/2.0);
	//printf("FAKTOR %f\n", factor);
	result.r[0] = current.v[0];
	result.r[1] = current.v[1];
	result.v[0] = -current.r[0] * (factor);
	result.v[1] = -current.r[1] * (factor);

	double mx1 = max(fabs(result.r[0]), fabs(result.r[1]));
	double mx2 = max(fabs(result.v[0]), fabs(result.v[1]));

	result.t = max(mx1, mx2);

	return result;
}

State RK4(State current, State(*genDifferentials) (State))
{
	// h: lépéshossz
	//static double h;
	//static double D;

	State k_1 = (*genDifferentials) (current);
	//puts("Nem a gendiff.");
	//puts("Nem a gendiff.");
	State intermediate = apply(current, k_1, 1.0 / 2.0 * h);
	//puts("Nem az apply.");
	State k_2 = (*genDifferentials) (intermediate);

	State intermediate2nd = apply(current, k_2, 1.0 / 2.0 * h);

	State k_3 = (*genDifferentials) (intermediate2nd);

	State intermediate3rd = apply(current, k_3, 1.0 * h);

	State k_4 = (*genDifferentials) (intermediate3rd);

	//printState(k_4);
	/*D = k_4.t * 1.0 / 6 + k_3.t * 1.0 / 3 + k_2.t * 1.0 / 3 +
	    k_1.t * 1.0 / 6;*/
	//printf()
	// printf("%f ANYÁÐ \n", D);

	State result =
	    apply(apply(apply(apply(current, k_1, 1.0 / 6 * h),
			      k_2, 1.0 / 3 * h),
			k_3, 1.0 / 3 * h),
		  k_4, 1.0 / 6 * h);

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

	return result;
}

int main(void)
{
	State startingConditions;

	startingConditions.r = (double *)malloc(sizeof(double) * 2);
	startingConditions.v = (double *)malloc(sizeof(double) * 2);
	startingConditions.r[0] = 363300.0;
	startingConditions.r[1] = 0.0;
	startingConditions.v[0] = 0.0;
	startingConditions.v[1] = 1.076;

	State current = startingConditions;
	State next;
	for (int i = 0; i < 10000; i++) {
		printf("Iteration: %d\th: %f |", i, h);
		printState(current);
		next = RK4(current, &moonsystem);
		free(current.r);
		free(current.v);
		current = next;
		//h = h * (pow(fabs(Dmax / D), (1.0 / 5)));

	}
	free(next.r);
	free(next.v);
	//h=h*(pow(fabs(Dmax/D), (1/5)));
}
