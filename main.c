#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct matrix
{
    int sor;
    int oszlop;
    double *adat;
} Matrix;

Matrix loadMatrix(FILE *infile, int oszlop)
{
    Matrix mat;
    mat.oszlop = oszlop;
    mat.adat = (double*)calloc(oszlop,sizeof(double));
    int i;
    int sorhanyadik=0;
    int err = 0;

    do
    {
        mat.adat = (double*)realloc(mat.adat,(sorhanyadik+1)*oszlop*sizeof(double));
        //err fordítva működik
        for( i = 0; i < mat.oszlop; i++)
        {
            err = fscanf(infile,"%lf",&(mat.adat[mat.oszlop * sorhanyadik  + i]));
        }

        if(!err)
        {
            mat.adat = (double*)realloc(mat.adat,(sorhanyadik)*oszlop*sizeof(double));
        }
        sorhanyadik++;
    }
    while(!feof(infile));
    mat.sor = sorhanyadik;
    return mat;
}

void Matrixkiir(Matrix mat)
{
    int k;
    int j;
    for(k=0; k<mat.sor; k++)
    {
        for(j=0; j<mat.oszlop; j++)
        {
            printf("%f ", mat.adat[k*mat.oszlop+j]);
        }
        printf("\n");
    }
}


Matrix matrixszorzas(Matrix bal, Matrix jobb)
{
    Matrix eredmeny;
    eredmeny.adat = (double*)calloc(bal.sor * jobb.oszlop,sizeof(double));
    eredmeny.sor = bal.sor;
    eredmeny.oszlop = jobb.oszlop;
    if(jobb.sor!=bal.oszlop)
    {
        printf("Ezek a mátrixok, nem szorozhatók! \n");
        eredmeny.sor=0;
        eredmeny.oszlop=0;
        eredmeny.adat=NULL;
        return eredmeny;
    }

    int i,j,k;
    for(i=0; i<bal.sor; ++i)
    {
        for(k=0; k<jobb.oszlop;  ++k)
        {
            for(j=0; j<bal.oszlop; ++j)
            {
                eredmeny.adat[i*jobb.oszlop+k] += bal.adat[i*bal.oszlop+j]*jobb.adat[j*jobb.oszlop+k];
            }
        }
    }

    return eredmeny;

}

int sormax(Matrix matrix, int sorhanyadik)
{
    int k;
    int hely=0;
    for(k=0; k<matrix.oszlop; k+=1)
    {
        if(fabs(matrix.adat[(sorhanyadik)*matrix.oszlop+k])>fabs(matrix.adat[(sorhanyadik)*matrix.oszlop+hely]))
        {
            hely=k;
        }
    }
    return hely;
}

void sorszorzas(Matrix mat, int sorhanyadik, double szam)
{
    int k;
    for(k=0; k<mat.oszlop; k+=1)
    {
        mat.adat[(sorhanyadik)*mat.oszlop+k]*=szam;
    }
}

void sorcsere(Matrix mat, int egyiksor, int masiksor)
{
    int k;
    for(k=0; k<mat.oszlop; k+=1)
    {
        double temp;
        temp=mat.adat[(egyiksor)*mat.oszlop+k];
        mat.adat[(egyiksor)*mat.oszlop+k]=mat.adat[(masiksor)*mat.oszlop+k];
        mat.adat[(masiksor)*mat.oszlop+k]=temp;
    }
}

int oszlopmax(Matrix mat, int oszlophanyadik,int sorhanyadik)
{
    int k;
    int hely=sorhanyadik;
    for(k=hely; k<=mat.sor; k+=1)
    {
        if(fabs(mat.adat[k*mat.oszlop+oszlophanyadik])>fabs(mat.adat[hely*mat.oszlop+oszlophanyadik]))
        {
            hely=k;
        }
    }
    return hely;
}

void sorosszead(Matrix mat, int egyiksor, int masiksor, double szam)
{
    int k;
    for(k=0; k<mat.oszlop; k+=1)
    {
        mat.adat[(egyiksor)*mat.oszlop+k]+=(mat.adat[(masiksor)*mat.oszlop+k])*szam;
    }
}

Matrix Trans(Matrix mat)
{
    Matrix eredmeny;
    eredmeny.sor = mat.oszlop;
    eredmeny.oszlop = mat.sor;
    eredmeny.adat = (double*)malloc(eredmeny.sor * eredmeny.oszlop*sizeof(double));
    int i,j;
    for(i = 0; i <mat.sor; i++)
    {
        for(j = 0; j < mat.oszlop; j++)
        {
            eredmeny.adat[j*eredmeny.oszlop+i] = mat.adat[i*mat.oszlop+j];
        }
    }
    return eredmeny;
}




void inverz(Matrix mat, Matrix egyseg)
{
    Matrix matrix;
    matrix.sor = matrix.oszlop = mat.sor;
    matrix.adat = (double*)calloc(matrix.sor * matrix.oszlop,sizeof(double));
    int k;
    for(k=0; k<matrix.sor*matrix.sor; k+=1)
    {
        matrix.adat[k] = mat.adat[k];
    }

    //normálás áttéve az eliminacioba
    double pivot;

    //elimináció
    int pivothely;
    for(k=0; k<matrix.sor; k+=1)
    {
        for(int z=k; z<matrix.sor; z+=1)
        {
            int p = sormax(matrix,z);
            pivot = matrix.adat[z*matrix.oszlop+p];
            sorszorzas(matrix, z, 1.0/pivot);
            sorszorzas(egyseg, z, 1.0/pivot);
        }

        pivothely = oszlopmax(matrix,k,k);
        if(fabs(matrix.adat[pivothely*matrix.oszlop+k])<0.000001)
        {
            printf("Ez a mátrix szinguláris! \n");
            exit(0);
        }
        sorcsere(matrix,pivothely,k);
        sorcsere(egyseg,pivothely,k);
        int i;
        for(i=k+1; i<matrix.sor; i+=1)
        {
            sorosszead(egyseg,i,k,-(matrix.adat[i*matrix.oszlop+k])/(matrix.adat[k*matrix.oszlop+k]));
            sorosszead(matrix,i,k,-(matrix.adat[i*matrix.oszlop+k])/(matrix.adat[k*matrix.oszlop+k]));
        }
    }

    //visszafelé
    for(k=matrix.sor-1; k>=0; k-=1)
    {
        int j;
        for(j=k-1; j>=0; j-=1)
        {
            sorosszead(egyseg,j,k,-(matrix.adat[j*matrix.oszlop+k])/(matrix.adat[k*matrix.oszlop+k]));
            sorosszead(matrix,j,k,-(matrix.adat[j*matrix.oszlop+k])/(matrix.adat[k*matrix.oszlop+k]));
        }
    }

    //normálás
    for(k=0; k<matrix.sor; k+=1)
    {
        sorszorzas(egyseg, k, 1.0/matrix.adat[k*matrix.oszlop+k]);
        sorszorzas(matrix, k, 1.0/matrix.adat[k*matrix.oszlop+k]);
    }
}


int main(int argc, char **argv)
{
    FILE *inp;
    inp = fopen(argv[3],"r");
    printf("elotte");
    Matrix mat = loadMatrix(inp, atoi(argv[1])+2);
    printf("utana");
    Matrix terv;
    terv.sor = mat.sor;
    terv.oszlop = atoi(argv[2])*atoi(argv[1])+1;
    terv.adat = (double*)malloc(terv.sor * terv.oszlop*sizeof(double));
    int meres, valtozo, kitevo;
    for(meres = 0; meres < mat.sor; meres++)
    {
        terv.adat[meres*terv.oszlop] = 1;
        for(valtozo = 0; valtozo < atoi(argv[1]); valtozo++)
        {
            for(kitevo =1; kitevo <= atoi(argv[2]); kitevo++)
            {
                terv.adat[meres*terv.oszlop+valtozo*atoi(argv[2])+kitevo] = pow(mat.adat[meres*mat.oszlop+valtozo],kitevo);
            }
        }
    }
    //Matrixkiir(terv);
    //printf("%i",terv.sor);
    //puts("\n");
    //szigmával majd még osztunk...
    Matrix b;
    b.oszlop = 1;
    b.sor = mat.sor;
    b.adat = (double*)malloc(b.sor *sizeof(double));
    int bsor;
    for(bsor = 0; bsor < mat.sor; bsor++)
    {
        b.adat[bsor] = mat.adat[bsor*mat.oszlop+mat.oszlop-2];
        //szigma
    }

    Matrix Tr = Trans(terv);
    //printf("transzponált \n");
    //Matrixkiir(Tr);
    Matrix Bal = matrixszorzas(Tr,terv);
    Matrix Jobb = matrixszorzas(Tr,b);
    //Matrixkiir(Bal);
    //puts("\n");
    //Matrixkiir(Jobb);
    //puts("\n");
    //Matrix jobb1 = Trans(Jobb);
    //Matrix naazert = Trans(jobb1);

    Matrix egyseg;
    egyseg.sor = egyseg.oszlop = Bal.sor;
    egyseg.adat = (double*)calloc(egyseg.sor * egyseg.oszlop,sizeof(double));
    int i;
    for(i=0; i<egyseg.sor; i+=1)
    {
        egyseg.adat[(i*egyseg.oszlop)+i] = 1.0;
    }

    /*Matrix Bali =*/ inverz(Bal,egyseg);
    Matrix megoldas = matrixszorzas(egyseg,Jobb);



    Matrixkiir(egyseg);
    puts("\n");
    Matrixkiir(megoldas);


    //free mat, terv, b
    return 0;
}
