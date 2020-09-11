#include <stdio.h>
#include <math.h>
#include <stdlib.h>

//
const double alpha = 7.911392;
//ryzen2 0.560268;
  const double f_0=-6.070866;
//ryzen2  1.06473

/*inline*/ double max(double a, double b)
{
	return a > b ? a : b;
}

typedef struct state {
	double u;
        double f;
	double n;
} State;

static double h = 0.02;
const static double Dmax = 0.0075;
static double D = 0.001;

void printState(State s)
{
  	printf("f: %f ", s.f);
	printf("U: %f ", s.u);
	printf("n: %f ", s.n);


}

State apply(State current, State differential, double multiplier)
{
	State result;

	// multiplier: 1/2*h, etc.
	
	result.f = current.f + multiplier * differential.f;
	result.u = current.u + multiplier * differential.u;

	result.n = current.n + multiplier;

	return result;
}

State moonsystem(State current)
{
	State result;
	result.f = -alpha/current.n/(alpha/current.f+2.0/current.u);
	result.u = -1.0/current.n/(alpha/current.f+2.0/current.u);

	result.n = max(result.f, result.u);

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

	return result;
}

int main(void)
{
	State startingConditions;

	startingConditions.f=3.7;
	startingConditions.u = (startingConditions.f-f_0)/alpha;
	  //bulldozer2 (startingConditions.f+2.97119)/5.29335;
	  //bulldozer1 (startingConditions.f+3.8795)/5.79039;
	startingConditions.n=6.0;

	State current = startingConditions;
	State next;
	int i = 0;
	while (next.n<8.0 ) {
		printf("Iteration: %d\th: %f |", i++, h);
		printState(current);
		putchar('\n');
		next = RK4(current, &moonsystem);
		current = next;
		//h = h * (pow(fabs(Dmax / D), (1.0 / 5)));

	}
	//h=h*(pow(fabs(Dmax/D), (1/5)));
}
