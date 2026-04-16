#ifndef INTERPOLATOR_H
#define INTERPOLATOR_H

typedef struct {
    double CL;
    double CD;
    double ALPHA;
    double Reynolds;
} Coeficients;


typedef struct {

    double CL;
    double CD;

} Interpolation;


Coeficients *load(int n, char arq_name[]);

double Lagrange_Polin(int n, double x[], double fx[], double target);

double linear_interp(int n, double x[], double fx[], double target);

Interpolation interpolator(int n, int line_number, char **arq_names, double RE_target, double alpha_target, Coeficients **coeficients);

#endif
