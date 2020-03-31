#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define TAV(x,y) (x*x+y*y)

typedef struct allapot
{
    double x;
    double y;
    double vx;
    double vy;
    double t;

} Allapot;


static const double Foldm = 5.9736E+24;
static const double Holdm = 7.349E+22;
static const double apog = 405500;
static const double apogv = 0.964;
static const double perig = 363300;
static const double perigv = 1.076;
static const double Gamma = 6.67384E-20;
static const double h = 1000;

double E(Allapot a)
{
    return Holdm*0.5*TAV(a.vx,a.vy)+Gamma*Holdm*Foldm/TAV(a.x,a.y);
}

Allapot dif(Allapot all)
{
    Allapot d;
    d.x = all.vx;
    d.y = all.vy;
    d.vx = -Gamma*Foldm/pow(TAV(all.x,all.y),1.5)*all.x;
    d.vy = -Gamma*Foldm/pow(TAV(all.x,all.y),1.5)*all.y;
    return d;
}

Allapot leptet(Allapot jelenlegi, Allapot diff, double szorzo)
{
    Allapot kovetkezo;
    kovetkezo.x = jelenlegi.x+szorzo*diff.x;
    kovetkezo.y = jelenlegi.y+szorzo*diff.y;
    kovetkezo.vx = jelenlegi.vx+szorzo*diff.vx;
    kovetkezo.vy = jelenlegi.vy+szorzo*diff.vy;
    kovetkezo.t = jelenlegi.t+szorzo;
    return kovetkezo;
}

Allapot euler(Allapot all, Allapot (*diffel)(Allapot))
{
    Allapot k = (*diffel)(all);
    return leptet(all, k, h);
}

int main()
{
    Allapot kezdeti;
    kezdeti.x = perig;
    kezdeti.y = 0;
    kezdeti.vx = 0;
    kezdeti.vy = perigv;
    kezdeti.t = 0;


    FILE *outp;
    outp = fopen("negy.txt", "w");

    int i;
    Allapot jelenlegi=kezdeti, kovi;
    for(i=0; i < 1000000; i++)
    {
        kovi=euler(jelenlegi,&dif);
        fprintf(outp, "%d %lf %lf %lf %lf %lf %lf\n", i, jelenlegi.x, jelenlegi.y, jelenlegi.vx, jelenlegi.vy, jelenlegi.t, E(jelenlegi));
        jelenlegi = kovi;
    }

    fclose(outp);

    return 0;
}
//fajlba kiir csak a szamok es abrazol
