#include "comparison.h"
#include "parser.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "parser.h"



ComparisonPoints comparison_points(){


    // arquivo
    FILE *arq;
    char l[256];

    // variaveis
    double *Cp;
    double *Ct;
    double *lambda;
    double *lambda_2;

    Cp = malloc(100 * sizeof(double));
    Ct = malloc(100 * sizeof(double));
    lambda = malloc(100 * sizeof(double));
    lambda_2 = malloc(100 * sizeof(double));


    // variaveis de funcionamento
    double content2[2];
    char CP[] = "Cp:";
    char CT[] = "Ct:";
    char *content;

    int idx_1 = 0;
    int idx_2 = 0;
    arq = fopen("comparison.txt", "r");                                     

    while (fgets(l, sizeof(l), arq) != NULL) {

        if (strncmp(l, CP, strlen(CP))==0){

            content = strchr(l, ':');
            remover_dois_pontos(content);
            int k = 0;
            int j = 0;
            char transient[100] = "";

            for (int i = 0; content[i] != '\0'; i++){
                if (content[i] != ' ' && content[i] != '\n'){
                    transient[k] = content[i];
                    k = k + 1;
                    transient[k] = '\0';
                } else {
                    content2[j] = strtof(transient, NULL);
                    transient[0] = '\0';
                    j = j + 1;
                    k = 0;
                }
            }

            lambda[idx_1] = content2[0];
            Cp[idx_1] = content2[1];

            idx_1++;

        }


        if (strncmp(l, CT, strlen(CT))==0){

            content = strchr(l, ':');
            remover_dois_pontos(content);
            int k = 0;
            int j = 0;
            char transient[100] = "";

            for (int i = 0; content[i] != '\0'; i++){
                if (content[i] != ' ' && content[i] != '\n'){
                    transient[k] = content[i];
                    k = k + 1;
                    transient[k] = '\0';
                } else {
                    content2[j] = strtof(transient, NULL);
                    transient[0] = '\0';
                    j = j + 1;
                    k = 0;
                }
            }

            lambda_2[idx_2] = content2[0];
            Ct[idx_2] = content2[1];

            idx_2++;

        }

    }


    Cp = realloc(Cp, idx_1*sizeof(double));
    lambda = realloc(lambda, idx_1*sizeof(double));

    Ct = realloc(Ct, idx_2*sizeof(double));
    lambda_2 = realloc(lambda_2, idx_2*sizeof(double));

    
    ComparisonPoints C;

    C.Cp = Cp;
    C.Lambda_Cp = lambda;
    C.size_Cp = idx_1;
    C.Ct = Ct;
    C.Lambda_Ct = lambda_2;
    C.size_Ct = idx_2;
    C.P = NULL;
    C.Lambda_P = NULL;
    C.size_P = 0;

    // CHECA CHECA
    //for (int i = 0; i < idx_1; i++){
    //    printf("\nCp[%d] = %f | lambda[%d] = %f", i, Cp[i], i, lambda[i]);
    //}
    //for (int i = 0; i < idx_2; i++){
    //    printf("\nCt[%d] = %f | lambda_2[%d] = %f", i, Ct[i], i, lambda_2[i]);
    //}

    return C;

}

//int main(){
//
//    comparison_points();
//    
//}
