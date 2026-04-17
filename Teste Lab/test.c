#include <stdlib.h>
#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>


double **DR(int AIRFOIL_NUMBER, int *sizes, double **radius){
    
    printf("\nEntrou na funcao");

    double **RADIUS_WITH_ZERO;
    double **dr;

    RADIUS_WITH_ZERO = malloc(AIRFOIL_NUMBER * sizeof(double *));
    dr = malloc(AIRFOIL_NUMBER * sizeof(double *));

    for (int i = 0; i < AIRFOIL_NUMBER; i++){

        RADIUS_WITH_ZERO[i] = malloc((sizes[i]+1) * sizeof(double));

    }

    for (int i = 0; i < AIRFOIL_NUMBER; i++){

        dr[i] = malloc(sizes[i] * sizeof(double));

    }

    RADIUS_WITH_ZERO[0][0] = 0;

    double LAST_VALUE = 0;

    printf("\nAlocou tudo na funcao");

    for (int j = 0; j < AIRFOIL_NUMBER; j++){ 

        for (int i = 0; i < sizes[j]; i++) {

            printf("\nEntrou nos fors");

            if (i == sizes[j] - 1){

                LAST_VALUE = radius[j][i];

                if (j < AIRFOIL_NUMBER - 1){

                    RADIUS_WITH_ZERO[j+1][0] = LAST_VALUE;

                }

            }

            RADIUS_WITH_ZERO[j][i+1] = radius[j][i];

            printf("\nTESTE");
            printf("RADIUS_WITH_ZERO[%d][%d] = %f", j, i, RADIUS_WITH_ZERO[j][i]);


            //if (k == 0){

            //    RADIUS_WITH_ZERO[j][i+1] = radius[j][i];

            //} else {

            //    RADIUS_WITH_ZERO[j][i] = radius[j][i];
            //}

        }
    }


    for (int j = 0; j < AIRFOIL_NUMBER; j++){

        for (int i = 0; i < sizes[j]; i++){

            dr[j][i] = RADIUS_WITH_ZERO[j][i+1] - RADIUS_WITH_ZERO[j][i];

        }

    }




    // CHECK
    for (int j = 0; j < AIRFOIL_NUMBER; j++){


        for (int i = 0; i < sizes[j]+1; i++) {
            printf("\nRADIUS[%d][%d] = %f", j, i, RADIUS_WITH_ZERO[j][i]);

        }
    }


    // CHECK
    for (int j = 0; j < AIRFOIL_NUMBER; j++){


        for (int i = 0; i < sizes[j]; i++) {
            printf("\ndr[%d][%d] = %f", j, i, dr[j][i]);

        }
    }
    return dr;
}



int main(){

    printf("\nEntrou na main");

    int AIRFOIL_NUMBER = 3;
    int sizes[3] = {1, 2, 3};
    double **radius;

    radius = malloc(3*sizeof(double *));

    for (int i = 0; i < AIRFOIL_NUMBER; i++){

        radius[i] = malloc(sizes[i] * sizeof(double));

    }

    printf("\nPassou da criacao de variaveis");

    int k = 30;
    for (int i = 0; i < AIRFOIL_NUMBER; i++){

        for(int j = 0; j < sizes[i]; j++){

            radius[i][j] = k;
            printf("\nradius[%d][%d] = %f", i, j, radius[i][j]);
            k++;
        }

    }


    printf("\nPassou da criacao do raio");

    DR(AIRFOIL_NUMBER, sizes, radius);


    return 0;



}
