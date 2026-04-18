#include <math.h>
#include "lib.h"
#include <stdbool.h>
#define _USE_MATH_DEFINES

double V_from_TSR(double TSR, double R, double OMEGA){
    return OMEGA*R/TSR;
}

double V_rel(double V, double a, double al, double OMEGA, double r){
    return sqrt(pow((1-a)*V, 2) + pow((1+al)*OMEGA*r,2));
}

double tan_phi(double a, double al, double V, double w, double r){
    return (1-a)*V/((1-al)*w*r);
}

double phi(double tan_phi){
    return atan(tan_phi);
}

double alfa(double phi, double theta, double pitch){
    return phi - theta - pitch;
}

double CN(double CL, double CD, double phi){
    return CL*cos(phi) + CD*sin(phi);
}

double CT(double CL, double CD, double phi){
    return CL*sin(phi) - CD*cos(phi);
}

double f(double B, double R, double r, double phi){
    return (B/2)*(R-r)/(r*sin(phi));
}

double F(double f){
    return (2/M_PI)*acos(exp(-f));
}

double Reynolds(double rho, double c, double mu, double V){
    return rho*c*V/mu;
}

double sigma(double c, double B, double r){
    return c*B/(2*M_PI*r);
}

double calc_a(double F, double phi, double sigma, double CN){
    double num;
    double den;
    num = 4*F*pow(sin(phi),2);
    den = sigma*CN;
    return 1/(num/den + 1);
}

double calc_al(double F, double phi, double sigma, double CT){
    double num;
    double den;
    num = 4*F*sin(phi)*cos(phi);
    den = sigma*CT;
    return 1/(num/den + 1);
}

double converged(double a, double a_i, double al, double al_i, double convergence_factor){
    double a_diff;
    double al_diff;
    a_diff = a - a_i;
    al_diff = al - al_i;
    if (sqrt(pow(a_diff, 2)) <= convergence_factor && sqrt(pow(al_diff, 2)) <= convergence_factor) {
        return true;
    } else {
        return false;
    }
}

double relax_a(double a, double a_i, double relax){
    return (1-relax)*a + relax*a_i;
}

double relax_al(double al, double al_i, double relax){
    return (1-relax)*al + relax*al_i;
}

double K_spera(double F, double phi, double sigma, double CN){

    double num;
    double den;

    num = 4*F*pow(sin(phi),2);
    den = sigma*CN;
    return num/den;
}

double spera_factor(double ac, double a, double F, double phi, double CN, double sigma){

    double K = K_spera(F, phi, sigma, CN);
    double A = 2;
    double B = K*(1-2*ac);
    double C = pow(K*(1-2*ac)+2, 2);
    double D = 4*(K*pow(ac, 2)-1);
    double E = sqrt(C+D);

    return 0.5*(A + B - E);
}
