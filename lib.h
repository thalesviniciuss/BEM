#ifndef LIB_H
#define LIB_H


double V_from_TSR(double TSR, double R, double OMEGA);

double V_rel(double V, double a, double al, double OMEGA, double r);

double tan_phi(double a, double al, double V, double w, double r);

double phi(double tan_phi);

double alfa(double phi, double theta, double pitch);

double CN(double CL, double CD, double phi);

double CT(double CL, double CD, double phi);

double f(double B, double R, double r, double phi);

double F(double f);

double Reynolds(double rho, double c, double mu, double V);

double sigma(double c, double B, double r);

double calc_a(double F, double phi, double sigma, double CN);

double calc_al(double F, double phi, double sigma, double CT);

double converged(double a, double a_i, double al, double al_i, double convergence_factor);

double relax_a(double a, double a_i, double relax);

double relax_al(double al, double al_i, double relax);

double K_spera(double F, double phi, double sigma, double CN);

double spera_factor(double ac, double a, double F, double phi, double CN, double sigma);

#endif
