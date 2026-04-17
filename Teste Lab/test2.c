#include "../BEM.h"
#include "../parser.h"
#include "../interpolator.h"
#include <stdlib.h>
#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>

typedef struct {
    double *T;
    double *M;
    double *P;
    double *Cp;
    double *Ct;
}  PhysicalQuantities;

    
PhysicalQuantities CALCULATE_PHYSICAL_QUANTITIES(int TIP_SPEED_RATIO_VECTOR_SIZE, double *TIP_SPEED_RATIO_VECTOR, int *sizes, Output ***solutions, double rho, double V_INF, double **radius, double **dr, int AIRFOIL_NUMBER, double R){

    PhysicalQuantities Phys;


    double **T_s;
    double **M_s;
    double *T;
    double *M;
    double *P;
    double *Cp;
    double *Ct;
    double *OMEGA;
    double dT = 0;
    double dM = 0;

    T = malloc(TIP_SPEED_RATIO_VECTOR_SIZE * sizeof(double));
    M = malloc(TIP_SPEED_RATIO_VECTOR_SIZE * sizeof(double));

    T_s = malloc(TIP_SPEED_RATIO_VECTOR_SIZE * sizeof(double *));
    M_s = malloc(TIP_SPEED_RATIO_VECTOR_SIZE * sizeof(double *));

    P = malloc(TIP_SPEED_RATIO_VECTOR_SIZE * sizeof(double));
    Cp = malloc(TIP_SPEED_RATIO_VECTOR_SIZE * sizeof(double));
    Ct = malloc(TIP_SPEED_RATIO_VECTOR_SIZE * sizeof(double));

    for (int j = 0; j < TIP_SPEED_RATIO_VECTOR_SIZE; j++){

        T_s[j] = malloc(AIRFOIL_NUMBER * sizeof(double));
        M_s[j] = malloc(AIRFOIL_NUMBER * sizeof(double));
    }

    OMEGA = malloc(TIP_SPEED_RATIO_VECTOR_SIZE * sizeof(double));

    for (int i = 0; i < TIP_SPEED_RATIO_VECTOR_SIZE; i++){

        OMEGA[i] =V_INF*TIP_SPEED_RATIO_VECTOR[i]/R; 

    }

     

    for (int k = 0; k < TIP_SPEED_RATIO_VECTOR_SIZE; k++){

        T[k] = 0;
        M[k] = 0;
        P[k] = 0;
        Cp[k] = 0;
        Ct[k] = 0;

        for (int j = 0; j < AIRFOIL_NUMBER; j++){

            T_s[k][j] = 0;
            M_s[k][j] = 0;

        }
    }



    for (int k = 0; k < TIP_SPEED_RATIO_VECTOR_SIZE; k++){

        for (int j = 0; j < AIRFOIL_NUMBER; j++){

            for (int i = 0; i < sizes[j]; i++){

                dT = 4*M_PI*rho*pow(V_INF, 2)*solutions[k][j][i].a*(1-solutions[k][j][i].a)*solutions[k][j][i].F*dr[j][i]*radius[j][i];
                dM = 4*M_PI*pow(radius[j][i], 3)*rho*V_INF*OMEGA[k]*(1-solutions[k][j][i].a)*solutions[k][j][i].al*solutions[k][j][i].F*dr[j][i];
                M_s[k][j] = M_s[k][j] + dM;
                T_s[k][j] = T_s[k][j] + dT;

            }
        }
    }



    for (int k = 0; k < TIP_SPEED_RATIO_VECTOR_SIZE; k++){

        for (int j = 0; j < AIRFOIL_NUMBER; j++){
            T[k] += T_s[k][j];
            M[k] += M_s[k][j];

        }
    }


    for (int j = 0; j < TIP_SPEED_RATIO_VECTOR_SIZE; j++){

        P[j] = OMEGA[j]*M[j];
        Cp[j] = P[j]/(0.5*rho*pow(V_INF, 3)*M_PI*pow(R,2));
        Ct[j] = T[j]/(0.5*rho*pow(V_INF, 2)*M_PI*pow(R,2));

    }



    Phys.T = T;
    Phys.M = M;
    Phys.P = P;
    Phys.Cp = Cp;
    Phys.Ct = Ct;

    return Phys;
}


int main(){


    return 0;
}
