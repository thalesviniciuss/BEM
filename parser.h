#ifndef PARSER_H
#define PARSER_H


typedef struct {
    char name[100];
    double start;
    double stop;
    char **arq_names;
} Airfoil;

typedef struct {
    int radius_length;
    int chord_length;
    int twist_length;
    double rho;
    double mu;
    double R;
    double B;
    double AIRFOIL_NUMBER;
    double ARQ_NUMBER;
    double *chord;
    double *radius;
    double *twist;
    Airfoil *airfoils;
    char *ARQ_NAMES;
} Data;



Data parser();

#endif
