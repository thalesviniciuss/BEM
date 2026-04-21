#ifndef COMPARATION_H
#define COMPARATION_H


typedef struct{

    double *Cp;
    double *Ct;
    double *P;
    double *Lambda_Cp;
    double *Lambda_Ct;
    double *Lambda_P;
    int size_Cp;
    int size_Ct;
    int size_P;

} ComparisonPoints;

ComparisonPoints comparison_points();

#endif
