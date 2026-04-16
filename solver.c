#include "BEM.h"
#include "parser.h"
#include "interpolator.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <plplot.h>


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


void GRAPHIC_NAME(char name[], int size){

    printf("DIGITE O NOME DO GRAFICO: ");
    fgets(name, size, stdin);
    name[strcspn(name, "\n")] = '\0';
}


void CREATE_GRAPHICS(char GNAME[]){

    plsdev("svg");
    snprintf(GNAME, sizeof(GNAME),
         "%s.svg",
         GNAME);
    plsfnam(GNAME);
    plscolbg(255, 255, 255);
    plscol0(1, 0, 0, 0);
    plssub(2, 1);
    plinit();


    PLFLT x[];
    PLFLT Cp_Graphic[];
    PLFLT Ct_Graphic[];
}


int main() {


    // BLOCO PARA LER 
    // VARIAVEIS DO CONSOLE

    int SIZE = 20;
    char GNAME[SIZE];

    GRAPHIC_NAME(GNAME, SIZE);


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

    //check(d, AIRFOIL_NUMBER, sizes, radius, chord, twist);

    double TIP_SPEED_RATIO = 6;
    double V_INF = 10.3;
    double OMEGA = (V_INF*TIP_SPEED_RATIO)/R;
    double PITCH = 0;
    int LINE_NUMBER = 78;

    //PROXIMO PASSO É IMPLEMENTACAO DO BEM
    
    Coeficients ***coeficients = malloc(AIRFOIL_NUMBER*d.ARQ_NUMBER*sizeof(Coeficients *));

    for (int j = 0; j < AIRFOIL_NUMBER; j++) {
        coeficients[j] = malloc(d.ARQ_NUMBER * sizeof(Coeficients *));
    }

    for (int j = 0; j < AIRFOIL_NUMBER; j++){

        for (int i = 0; i < d.ARQ_NUMBER; i++){

            coeficients[j][i] = load(LINE_NUMBER, d.airfoils[j].arq_names[i]);
        }
    }
    

    Output *solution;


    solution = BEM(d.ARQ_NUMBER, LINE_NUMBER, d.airfoils[0].arq_names, sizes[0], rho, mu, TIP_SPEED_RATIO, OMEGA, R, twist[0], B, chord[0], radius[0], PITCH, coeficients[0]);
    
    double T = 0;
    double M = 0;
    double dT = 0;
    double dM = 0;
    double P = 0;
    double Cp = 0;
    double Ct = 0;

    double RADIUS_WITH_ZERO[sizes[0]+1];
    RADIUS_WITH_ZERO[0] = 0;

    double dr[sizes[0]];

    for (int i = 0; i < sizes[0]; i++) {

        RADIUS_WITH_ZERO[i+1] = radius[0][i]; 

    }

    for (int i = 0; i < sizes[0]; i++){

        dr[i] = radius[0][i] - RADIUS_WITH_ZERO[i];
    }

    for (int i = 0; i < sizes[0]; i++){

        dT = 4*M_PI*rho*pow(V_INF, 2)*solution[i].a*(1-solution[i].a)*solution[i].F*dr[i]*radius[0][i];
        T = T + dT;
        dM = 4*M_PI*pow(radius[0][i], 3)*rho*V_INF*OMEGA*(1-solution[i].a)*solution[i].al*solution[i].F*dr[i];
        M = M + dM;
    }


    P = OMEGA*M;
    Cp = P/(0.5*rho*pow(V_INF, 3)*M_PI*pow(R,2));
    Ct = T/(0.5*rho*pow(V_INF, 2)*M_PI*pow(R,2));


    printf("\nCHECK POS PROCESS P = %f | Cp = %f | Thrust ou Empuxo = %f | Ct = %f | Momento = %f", P, Cp, T, Ct, M);

    //printf("test test %f", solution[0].a);
    //check_BEM(solution, radius_length);

    //check(d, AIRFOIL_NUMBER, sizes, radius, chord, twist);



    // CALCULO PARA O VETOR DE TIP SPEED RATIO
    //
    
    double TIP_SPEED_RATIO_VECTOR[10] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 10.5};
    double OMEGA_V[10];

    for (int i = 0; i < 10; i++){

        OMEGA_V[i] =V_INF*TIP_SPEED_RATIO_VECTOR[i]/R; 

    }

    Output **SOLUTION_TIP_SPEED_RATIO_VECTOR = malloc(10*sizeof(Output *));

    for (int i = 0; i < 10; i++){

        SOLUTION_TIP_SPEED_RATIO_VECTOR[i] = BEM(d.ARQ_NUMBER, LINE_NUMBER, d.airfoils[0].arq_names, sizes[0], rho, mu, TIP_SPEED_RATIO_VECTOR[i], OMEGA_V[i], R, twist[0], B, chord[0], radius[0], PITCH, coeficients[0]);

    }

    
    double T_V[10];
    double M_V[10];
    double dT_V = 0;
    double dM_V = 0;
    double P_V[10];
    double Cp_V[10];
    double Ct_V[10];


    for (int j = 0; j < 10; j++){

        for (int i = 0; i < sizes[0]; i++){

            dT_V = 4*M_PI*rho*pow(V_INF, 2)*SOLUTION_TIP_SPEED_RATIO_VECTOR[j][i].a*(1-SOLUTION_TIP_SPEED_RATIO_VECTOR[j][i].a)*SOLUTION_TIP_SPEED_RATIO_VECTOR[j][i].F*dr[i]*radius[0][i];
            dM_V = 4*M_PI*pow(radius[0][i], 3)*rho*V_INF*OMEGA_V[j]*(1-SOLUTION_TIP_SPEED_RATIO_VECTOR[j][i].a)*SOLUTION_TIP_SPEED_RATIO_VECTOR[j][i].al*SOLUTION_TIP_SPEED_RATIO_VECTOR[j][i].F*dr[i];
            M_V[j] = M_V[j] + dM_V;
            T_V[j] = T_V[j] + dT_V;

        }
    }

    for (int j = 0; j < 10; j++){

        P_V[j] = OMEGA_V[j]*M_V[j];
        Cp_V[j] = P_V[j]/(0.5*rho*pow(V_INF, 3)*M_PI*pow(R,2));
        Ct_V[j] = T_V[j]/(0.5*rho*pow(V_INF, 2)*M_PI*pow(R,2));

    }

    // BLOCO DE CRIACAO DE GRAFICOS




    // CHECA POS PROCESSAMENTO
    
    for (int j = 0; j < 10; j++){

        printf("\nTSR[%d] = %f | P[%d] = %f | Cp[%d] = %f | T[%d] = %f | Ct[%d] = %f", j, TIP_SPEED_RATIO_VECTOR[j], j, P_V[j], j, Cp_V[j], j, T_V[j], j, Ct_V[j]);
    }

}

