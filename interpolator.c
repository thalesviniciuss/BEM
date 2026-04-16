#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include "interpolator.h"


// ANTES DE INTERPOLAR EU PRECISO CARREGAR
// NA MEMORIA TODOS OS ALPHA, CL E CD CALCULADOS
// PARA CADA REYNOLDS
// DEPOIS DISSO, FICARA FACIL INTERPOLAR ELES COM OS DADOS
// JA CARREGADOS NA MEMORIA


Coeficients *load(int n, char arq_name[]){

    Coeficients *vetor = malloc(n*sizeof(Coeficients));

    int line_counter = 1;

    FILE *arq;
    char l[256];
    arq = fopen (arq_name, "r"); // para leitura

    if (arq == NULL){
        printf("ERRO AO ABRIR O ARQUIVO");
        return vetor;
    }

    int z = 0;

    char RE[100];

    while (fgets(l, sizeof(l), arq) != NULL) {

        if (line_counter == 9) {
            int k = 0;
            for (int j = 0; j<sizeof(l); j++){
                if (j >= 29 && j<= 37){
                    if (l[j] != ' '){
                        RE[k] = l[j];
                        k++;
                        RE[k] = '\0';
                    }
                }
            }
        }


        if (line_counter >= 13) {

            // LOGICA PARA ARMAZENAR OS VALORES
            // ALPHA
            // CL
            // CD

            vetor[z].Reynolds = strtof(RE, NULL);

            char t_alpha[20] = "";
            char t_CL[20] = "";
            char t_CDp[20] = "";

            int a = 0;
            int b = 0;
            int c = 0;
            for (int j = 0; j<sizeof(l); j++){

                if (j >=1 && j <= 7){

                    t_alpha[a] = l[j];
                    a++;
                    t_alpha[a] = '\0';
                    //printf("\n teste g = %d e %c", g, l[j]);

                }

                if (j >=10 && j <= 16){

                    t_CL[b] = l[j];
                    b++;
                    t_CL[b] = '\0';

                }

                if (j >=20 && j <= 27){

                    t_CDp[c] = l[j];
                    c++;
                    t_CDp[c] = '\0';

                }


            }

            vetor[z].ALPHA = strtof(t_alpha, NULL);
            vetor[z].CL = strtof(t_CL, NULL);
            vetor[z].CD = strtof(t_CDp, NULL);
            
            z++;

            t_alpha[0] = '\0';
            t_CL[0] = '\0';
            t_CDp[0] = '\0';

        }

        line_counter++;
    }

    // TESTES
    //
    //
    for (int i = 0; i < n; i++){

        printf("\n z = %d | Reynolds = %f | Alpha = %f | CL = %f | CD = %f", i, vetor[i].Reynolds, vetor[i].ALPHA, vetor[i].CL, vetor[i].CD);
    }
    return vetor;
}



double Lagrange_Polin(int n, double x[], double fx[], double target){

    
    // NAO BATEU TAO BEM
    double L[n];
    double num = 1;
    double den = 1;


    for (int k = 0; k < n; k++){
        for (int i = 0; i < n; i++){
            if (i != k){
                num = (target - x[i])*num;
                den = (x[k]-x[i])*den;
            }
        }

        L[k] = num/den;
        num = 1;
        den = 1;
    }


    double response = 0;

    for (int j = 0; j < n; j++){
        response = fx[j]*L[j] + response;
    }

    return response;

}



double linear_interp(int n, double x[], double fx[], double target){


    // CORRENTE EM USO
    int minor_index = 0;
    int major_index = 0;

    //if (target <= x[0]) return fx[0];
    //if (target >= x[n-1]) return fx[n-1];

    bool maj_index_bool = false;

    for(int i = 0; i < n; i++){
        if (x[i] < target){
            minor_index = i;
        } else if (maj_index_bool == false){
            major_index = i;
            maj_index_bool = true;
        }
    }

    double f0;
    double f1;
    double x1;
    double x0;
    double response;


    f0 = fx[minor_index];
    f1 = fx[major_index];
    x0 = x[minor_index];
    x1 = x[major_index];
    response = f0 + ((f1-f0)/(x1-x0))*(target-x0);

    return response;
}


Interpolation interpolator(int n, int line_number, char **arq_names, double RE_target, double alpha_target, Coeficients **outputs){

    // INTERPOLAR PRIMEIRO O VALOR E DEPOIS PARA REYNOLDS
    //
    // ADICIONAR OUTPUT COMO ENTRADA E LER UMA VEZ SÓ OS ARQUIVOS
    
    
    double REs[n];
    double CLs[n];
    double CDs[n];

    //Coeficients **outputs = malloc(n*sizeof(Coeficients *));

    for (int i = 0; i < n; i++){
        //printf("\nANTES load %s", arq_names[i]);
        //outputs[i] = load(line_number, arq_names[i]);
        //printf("\nDEPOIS load %s", arq_names[i]);
        printf("\nn = %d | i = %d", n, i);
        REs[i] = outputs[i][0].Reynolds;
        printf("\nREYNOLDS OUTPUT %f", REs[i]);

        double alpha_list[line_number];
        double cl_list[line_number];
        double cd_list[line_number];

        for (int j = 0; j < line_number; j++){

            alpha_list[j] = outputs[i][j].ALPHA;
            cl_list[j] = outputs[i][j].CL;
            cd_list[j] = outputs[i][j].CD;
        }


        CLs[i] = linear_interp(line_number, alpha_list, cl_list, alpha_target); 
        CDs[i] = linear_interp(line_number, alpha_list, cd_list, alpha_target); 
    }


    double CL_final;
    double CD_final;

    CL_final = linear_interp(n, REs, CLs, RE_target);
    CD_final = linear_interp(n, REs, CDs, RE_target);

    Interpolation Data;

    Data.CL = CL_final;
    Data.CD = CD_final;

    //for (int i = 0; i < n; i++) {
    //    free(outputs[i]);
    //}
    //free(outputs);

    return Data;

}


//int main() {


//    int arq_numbers = 8;

//    char *arq_names[] = {
//    "RE5E3.txt",
//    "RE1E4.txt",
//    "RE3E4.txt",
//    "RE5E4.txt",
//    "RE75E4.txt",
//    "RE1E5.txt",
//    "RE15E5.txt",
//    "RE3E5.txt"
//};


//    int line_number = 78;


//    Interpolation output;


//    output = interpolator(arq_numbers, line_number, arq_names, 9233.32, 20);

//    printf("\nTESTE INTERPOLATOR CL %f", output.CL);
//    printf("\nTESTE INTERPOLATOR CD %f", output.CD);

//}
