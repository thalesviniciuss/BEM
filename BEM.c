#include "lib.h"
#include "BEM.h"
#include <math.h>
#include "interpolator.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#define _USE_MATH_DEFINES


Output *BEM(int arq_numbers, int line_number, char **arq_names, int n, double rho, double mu, double TSR, double OMEGA, double R, double beta[], double B, double c[], double r[], double pitch, Coeficients **coeficients){

    // n é a quantidade de seções de raio a ser calculada
    // nessa implementacao eu priorizei a simplicidade
    // entao tenho que passar um TIP SPEED RATIO por vez
    // e fazer um loop
    
    Output *vetor = malloc(n*sizeof(Output));

    bool CONVERGENCE = false;

    double a = 0;
    double al = 0;
    double ac = 0.2;

    int iteration = 0;

    double convergence_factor = 0.0001;
    double relax = 0.1;

    double c_i;
    double r_i;

    double V_i;
    double tanphi_i;
    double phi_i;
    double f_i;
    double F_i;
    double alfa_i;
    double beta_i;
    double V_rel_i;
    double Re_i;
    double CL_i;
    double CD_i;
    double CN_i;
    double CT_i;
    double sigma_i;

    double a_i;
    double al_i;

    Interpolation interp;
    const double phi_min = 1E-4;
    double num;
    double den;


    for (int i = 0; i < n; i++){

        while (CONVERGENCE != true && iteration < 600){

            iteration++;

            c_i = c[i];
            r_i = r[i];
            printf("\nTESTE n = %d | r[%d] = %f | a = %f | al = %f | iteration = %d", n, i, r_i, a, al, iteration);
            beta_i = beta[i];

            V_i = V_from_TSR(TSR, R, OMEGA);
            //tanphi_i = tan_phi(a, al, V_i, OMEGA, r_i);
            //phi_i = phi(tanphi_i);
            num = (1-a)*V_i;
            den = ((1+al)*OMEGA*r_i);
            phi_i = atan2(num, den);

            if (fabs(phi_i) < phi_min){
                phi_i = (phi_i>= 0.0) ? phi_min : -phi_min;
            }

            if (isnan(tanphi_i) || isinf(tanphi_i)) {
                printf("\nERRO: tanphi_i invalido");
                break;
            }

            if (isnan(phi_i) || isinf(phi_i)) {
                printf("\nERRO: phi_i invalido %f", phi_i);
                phi_i = phi_min; 
            }

            //printf("\n TESTE phi = %f", phi_i*180/M_PI);
            f_i = f(B, R, r_i, phi_i);
            //printf("\n TESTE f = %f", f_i);
            F_i = F(f_i);
            //printf("\n TESTE F = %f", F_i);
            alfa_i = alfa(phi_i, beta_i, pitch);
            //printf("\n TESTE alpha = %f", alfa_i*180/M_PI);

            if (alfa_i < -15*M_PI/180){

                alfa_i = (-15*M_PI/180);

            } else if (alfa_i > 20*M_PI/180){

                alfa_i = (20*M_PI/180);

            }

            //if ((alfa_i*180/M_PI) > 25){
            //    alfa_i = 25*M_PI/180;}
            //printf("\nPASSOU A");

            V_rel_i = V_rel(V_i, a, al, OMEGA, r_i);

            //printf("\nPASSOU B | V_rel_i = %f", V_rel_i);

            Re_i = Reynolds(rho, c_i, mu, V_rel_i);

            //printf("\nPASSOU C | Re_i = %f | alpha_deg = %f", Re_i, alfa_i*180/M_PI);

            interp = interpolator(arq_numbers, line_number, arq_names, Re_i, (alfa_i*180/M_PI), coeficients);

            //printf("\nPASSOU D | CL = %f | CD = %f", interp.CL, interp.CD);

            //interp = interpolator(arq_numbers, line_number, arq_names, 150000, (alfa_i*180/M_PI));
            //printf("\nCHECK INTERPOLATOR arq_numbers = %d | line_number = %d | Re_i = %f | alfa_i = %f | ", arq_numbers, line_number, Re_i, (alfa_i*180/M_PI));
            //

            CL_i = interp.CL;
            CD_i = interp.CD;

            //printf("\nPASSOU E");

            CN_i = CN(CL_i, CD_i, phi_i);

            //printf("\nPASSOU F");

            CT_i = CT(CL_i, CD_i, phi_i);

            //printf("\nPASSOU G");

            sigma_i = sigma(c_i, B, r_i);

            //printf("\nPASSOU H | sigma = %f", sigma_i);

            a_i = calc_a(F_i, phi_i, sigma_i, CN_i);

            //printf("\nPASSOU I | a_i = %f", a_i);

            al_i = calc_al(F_i, phi_i, sigma_i, CT_i);

            //printf("\nPASSOU J | al_i = %f", al_i);

            // BUHL CALCULATION
            //double dCT_i = 0;
            //dCT_i = buhl_dCT(sigma_i, a_i, CL_i, CD_i, phi_i);
            //if (dCT_i > 0.96*F_i){
            //    a_i = buhl_calculation_for_turbulent_wake_state(F_i, dCT_i);
            //}

            // SPERA CALCULATION
            if (a_i >= ac){
                a_i = spera_factor(ac, a_i, F_i, phi_i, CN_i, sigma_i);
            }

            //printf("\nTESTE n = %d | r[%d] = %f | a = %f | V = %f | tan = %f | alfa = %f | CL = %f | CD = %f | RE = %f", n, i, r_i, a, V_i, tanphi_i, alfa_i, CL_i, CD_i, Re_i);

            CONVERGENCE = converged(a, a_i, al, al_i, convergence_factor);

            a = relax_a(a, a_i, relax);
            al = relax_al(al, al_i, relax);

        }
        iteration = 0;
        CONVERGENCE = false;

        // CRIAR LOGICA PARA SALVAR TUDO QUE PRECISO
        
        vetor[i].a = a;
        vetor[i].al = al;
        vetor[i].F = F_i;
        vetor[i].lambda = TSR;
        vetor[i].omega = OMEGA;
        vetor[i].r = r_i;
        //a = 0;
        //al = 0;
    }
    return vetor;
}
