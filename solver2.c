#include "BEM.h"
#include "parser.h"
#include "interpolator.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <plplot.h>

typedef struct {
    double *T;
    double *M;
    double *P;
    double *Cp;
    double *Ct;
}  PhysicalQuantities;

void check(Data d, int AIRFOIL_NUMBER, int *sizes, double **radius, double **chord, double **twist){

    for (int i = 0; i < AIRFOIL_NUMBER; i++) {
        printf("\nAIRFOIL %d -> start=%f stop=%f name=%s",
                i, d.airfoils[i].start, d.airfoils[i].stop, d.airfoils[i].name);
    }

    for (int i = 0; i < AIRFOIL_NUMBER; i++){
        for (int j = 0; j < sizes[i]; j++){
            printf("\n CHECK radius[%d][%d] = %f | chord[%d][%d] = %f | twist[%d][%d] = %f", i, j, radius[i][j], i, j, chord[i][j], i, j, twist[i][j]);
        }
    }
}


void check_BEM(Output *sol, int radius_length){

    for (int i = 0; i < radius_length; i++){

        printf("\n CHECK a[i] = %f | al[i] = %f | TSR[i] = %f | OMEGA[i] = %f | r[i] = %f | F[i] = %f", sol[i].a, sol[i].al, sol[i].lambda, sol[i].omega, sol[i].r, sol[i].F);

    }
    
}


void GRAPHIC_NAME(char name[]){
    printf("DIGITE O NOME DO GRAFICO: ");
    scanf("%s", name);
}


void CREATE_GRAPHICS(int VECTOR_SIZE, char GNAME[], double *x, double *y, char X_TITLE[], char Y_TITLE[]){

    char filename[256];
    snprintf(filename, sizeof(filename),
         "%s.svg",
         GNAME);
    printf("\nSalvando arquivo: %s\n", filename);
    plsdev("svg");
    plsfnam(filename);
    plscolbg(255, 255, 255);
    plscol0(1, 0, 0, 0);
    plssub(1, 1);
    plinit();

    PLFLT X_G[VECTOR_SIZE];
    PLFLT Y_G[VECTOR_SIZE];
    double MAX_Y = 0;
    double MIN_Y = 0;

    // PASSA OS VALORES DE DOUBLE
    // PARA A VARIAVEL PROPRIA
    // DO GRAFICO
    for (int i = 0; i < VECTOR_SIZE; i++){
        X_G[i] = x[i];
        Y_G[i] = y[i]; 

        // PEGA O VALOR MAX
        // DO VETOR Y
        if (Y_G[i] > MAX_Y){
            MAX_Y = Y_G[i];
        }


        // PEGA O VALOR MIN
        // DO VETOR Y
        if (Y_G[i] < MIN_Y){
            MIN_Y = Y_G[i];
        }
    }

    plenv(X_G[0]-1, X_G[VECTOR_SIZE-1]+1, MIN_Y, MAX_Y, 0, 0);
    pllab(X_TITLE, Y_TITLE, "");
    plline(VECTOR_SIZE, X_G, Y_G);
    plend();

}


Output ***SOLVE_FOR_ANY_NUMBER_AIRFOILS(Data d, 
                                       int AIRFOIL_NUMBER,
                                       int LINE_NUMBER,
                                       int *sizes,
                                       double rho,
                                       double mu, 
                                       double *TIP_SPEED_RATIO,
                                       int TSR_SIZE, 
                                       double *OMEGA,
                                       double R, 
                                       double **twist,
                                       int B, 
                                       double **chord,
                                       double **radius,
                                       double PITCH,
                                       Coeficients ***coeficients){

    // CRIA O VETOR DE VETORES DE SOLUCOES
    // CADA SECAO OU AEROFOLIO POSSUI UM VETOR DE SOLUCOES
    // QUE SERA CALCULADO PELA FUNCAO BEM
    Output ***solutions;
    solutions = malloc(TSR_SIZE * sizeof(Output **));

    for (int k = 0; k < TSR_SIZE; k++) {
        solutions[k] = malloc(AIRFOIL_NUMBER * sizeof(Output *));
    }

    // ESSE EH O LOOP QUE SOLUCIONA PARA CADA AEROFOLIO
    // E PARA CADA TIP SPEED RATIO
    for (int j = 0; j < TSR_SIZE; j++){

        for (int i = 0; i < AIRFOIL_NUMBER; i++){

            solutions[j][i] = BEM(d.ARQ_NUMBER, LINE_NUMBER,
                    d.airfoils[i].arq_names,
                    sizes[i], rho,
                    mu, TIP_SPEED_RATIO[j],
                    OMEGA[j], R, twist[i],
                    B, chord[i], radius[i],
                    PITCH, coeficients[i]);
        }
    }

    return solutions;
}



double **DR(int AIRFOIL_NUMBER, int *sizes, double **radius){
    
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

    for (int j = 0; j < AIRFOIL_NUMBER; j++){ 

        for (int i = 0; i < sizes[j]; i++) {

            if (i == sizes[j] - 1){

                LAST_VALUE = radius[j][i];

                if (j < AIRFOIL_NUMBER - 1){

                    RADIUS_WITH_ZERO[j+1][0] = LAST_VALUE;

                }
            }

            RADIUS_WITH_ZERO[j][i+1] = radius[j][i];
        }
    }

    for (int j = 0; j < AIRFOIL_NUMBER; j++){

        for (int i = 0; i < sizes[j]; i++){

            dr[j][i] = RADIUS_WITH_ZERO[j][i+1] - RADIUS_WITH_ZERO[j][i];

        }

    }

    return dr;
}

    
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

// LE A VELOCIDADE INFINITA
void READ_V_INF(double *V_INF){
    printf("DIGITE A VELOCIDADE INFINITA: ");
    scanf("%lf", V_INF);
}

// LE O PITCH
// EM GRAUS
void READ_PITCH(double *PITCH){
    printf("DIGITE O PITCH EM GRAUS: ");
    scanf("%lf", PITCH);
}


int main() {


    // BLOCO PARA LER 
    // VARIAVEIS DO CONSOLE

    int LINE_NUMBER = 80;
    int SIZE = 20;
    char GNAME[SIZE];
    double V_INF = 10.3;
    double PITCH = 0;


    //READ_V_INF(&V_INF);
    //READ_PITCH(&PITCH);

    //PITCH = PITCH*180/M_PI;

    Data d;

    d = parser();

    double rho;
    double mu;
    double R;
    double B;
    double AIRFOIL_NUMBER;
    double **chord;
    double **radius;
    double **twist;
    int radius_length = d.radius_length;

    rho = d.rho;
    mu = d.mu;
    R = d.R;
    B = d.B;
    AIRFOIL_NUMBER = d.AIRFOIL_NUMBER;
    double airfoil_vector[(int)AIRFOIL_NUMBER];

    // ALOCACAO DE MEMORIA

    radius = malloc((int)AIRFOIL_NUMBER*sizeof(double *));
    chord = malloc((int)AIRFOIL_NUMBER*sizeof(double *));
    twist = malloc((int)AIRFOIL_NUMBER*sizeof(double *));

    for (int i = 0; i < AIRFOIL_NUMBER; i++){
        radius[i] = malloc(100*sizeof(double));
        chord[i] = malloc(100*sizeof(double));
        twist[i] = malloc(100*sizeof(double));
    }

    int sizes[(int)AIRFOIL_NUMBER];

    for (int i = 0; i < AIRFOIL_NUMBER; i++){
        int k = 0;
        for (int j = 0; j < radius_length; j++){
            //TESTE TESTE
            //printf("\nR=%f i=%d j=%d radius=%f start=%f stop=%f radius/R =%f",
            //   R, i, j, d.radius[j], d.airfoils[i].start, d.airfoils[i].stop, d.radius[j]/R);
            if (d.radius[j]/R >= (d.airfoils[i].start) && d.radius[j]/R <= (d.airfoils[i].stop)){
                radius[i][k] = d.radius[j];
                chord[i][k] = d.chord[j];
                twist[i][k] = d.twist[j];
                k++;
            }
        }
        sizes[i] = k;
        radius[i] = realloc(radius[i], (k)*sizeof(double));
        chord[i] = realloc(chord[i], (k)*sizeof(double));
        twist[i] = realloc(twist[i], (k)*sizeof(double));
    }

    // check(d, AIRFOIL_NUMBER, sizes, radius, chord, twist);

    // PROXIMO PASSO É IMPLEMENTACAO DO BEM
    // IMPLEMENTACAO DO BEM
    
    Coeficients ***coeficients = malloc(AIRFOIL_NUMBER*d.ARQ_NUMBER*sizeof(Coeficients *));

    for (int j = 0; j < AIRFOIL_NUMBER; j++) {
        coeficients[j] = malloc(d.ARQ_NUMBER * sizeof(Coeficients *));
    }

    for (int j = 0; j < AIRFOIL_NUMBER; j++){

        for (int i = 0; i < d.ARQ_NUMBER; i++){

            coeficients[j][i] = load(LINE_NUMBER, d.airfoils[j].arq_names[i]);
        }
    }
    

    Output ***solutions;
    double **dr;
    PhysicalQuantities Phys;

    int TSR_SIZE = 10;
    double *OMEGA_VECTOR;

    OMEGA_VECTOR = malloc(TSR_SIZE * sizeof(double));

    double TIP_SPEED_RATIO_VECTOR[10] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 10.5};

    for (int i = 0; i < TSR_SIZE; i++){
        OMEGA_VECTOR[i] = V_INF*TIP_SPEED_RATIO_VECTOR[i]/R;
    }


    solutions = SOLVE_FOR_ANY_NUMBER_AIRFOILS(d,
                                              AIRFOIL_NUMBER,
                                              LINE_NUMBER,
                                              sizes,
                                              rho,
                                              mu,
                                              TIP_SPEED_RATIO_VECTOR,
                                              TSR_SIZE,
                                              OMEGA_VECTOR,
                                              R,
                                              twist,
                                              B,
                                              chord,
                                              radius,
                                              PITCH,
                                              coeficients);

    dr = DR(AIRFOIL_NUMBER, sizes, radius);
    Phys = CALCULATE_PHYSICAL_QUANTITIES(TSR_SIZE,
                                         TIP_SPEED_RATIO_VECTOR,
                                         sizes, 
                                         solutions,
                                         rho,
                                         V_INF,
                                         radius,
                                         dr, 
                                         AIRFOIL_NUMBER,
                                         R);



    // CHECA POS PROCESSAMENTO
    
    for (int j = 0; j < TSR_SIZE; j++){

        printf("\nTSR[%d] = %f | P[%d] = %f | Cp[%d] = %f | T[%d] = %f | Ct[%d] = %f", j, TIP_SPEED_RATIO_VECTOR[j], j, Phys.P[j], j, Phys.Cp[j], j, Phys.T[j], j, Phys.Ct[j]);

    }

    // GRAFICO
    char X_TITLE[20];
    char Y_TITLE[20];
    int answer = 10;
    while (answer != 0){

        printf("\nQual grafico deseja criar?");
        printf("\n1 - Cp");
        printf("\n2 - Ct");
        printf("\n3 - P");
        printf("\nDigite 0 para sair: ");
        scanf("%d", &answer);


        switch (answer){

            case 1:
                GRAPHIC_NAME(GNAME);
                strcpy(X_TITLE, "TSR");
                strcpy(Y_TITLE, "Cp");
                CREATE_GRAPHICS(TSR_SIZE, GNAME, TIP_SPEED_RATIO_VECTOR, Phys.Cp, X_TITLE, Y_TITLE);
                break;

            case 2:
                GRAPHIC_NAME(GNAME);
                strcpy(X_TITLE, "TSR");
                strcpy(Y_TITLE, "Ct");
                CREATE_GRAPHICS(TSR_SIZE, GNAME, TIP_SPEED_RATIO_VECTOR, Phys.Ct, X_TITLE, Y_TITLE);
                break;

            case 3:
                GRAPHIC_NAME(GNAME);
                strcpy(X_TITLE, "TSR");
                strcpy(Y_TITLE, "P");
                CREATE_GRAPHICS(TSR_SIZE, GNAME, TIP_SPEED_RATIO_VECTOR, Phys.P, X_TITLE, Y_TITLE);
                break;
            case 0:
                break;
        }
    }



}

