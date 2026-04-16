#include "interpolator.h"
#ifndef BEM_H
#define BEM_H

typedef struct {

    double a;
    double al;
    double lambda;
    double omega;
    double r;
    double F;

} Output;


Output *BEM(int arq_numbers, int line_number, char **arq_names, int n, double rho, double mu, double TSR, double OMEGA, double R, double beta[], double B, double c[], double r[], double pitch, Coeficients **outputs);


#endif
