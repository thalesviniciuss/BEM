#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "parser.h"

void remover_dois_pontos(char str[]) {
    int i = 0, j = 0;
    while (str[i] != '\0') {
        if (str[i] != ':') {
            str[j] = str[i];
            j++;
        }
        i++;
    }
    str[j] = '\0';
}


Data parser(){

    Data d;
    d.rho = 0;
    d.mu = 0;
    d.R = 0;
    d.B = 0;
    d.AIRFOIL_NUMBER = 0;
    d.ARQ_NUMBER = 0;
    d.chord = NULL;
    d.radius = NULL;
    d.twist = NULL;

    FILE *arq;
    char l[256]; // linha

    char rho[] = "rho";
    char mu[] = "mu";
    char R[] = "R";
    char B[] = "B";
    char chord[] = "chord";
    char radius[] = "radius";
    char twist[] = "twist";
    char AIRFOIL_NUMBER[] = "AIRFOIL_NUMBER";
    char airfoil_parser[] = "AIRFOIL_CARAC";
    char ARQ_NUMBER[] = "ARQ_NUMBER";
    char XFOIL[] = "XFOIL";
    char *content;
    char *ARQ_NAMES;

    double rho2;
    double mu2;
    double R2;
    double B2;
    double AIRFOIL_NUMBER2;
    double ARQ_NUMBER2;

    double *chord2 = malloc(100*sizeof(double));
    double *radius2 = malloc(100*sizeof(double));
    double *twist2 = malloc(100*sizeof(double));
    Airfoil *airfoils2 = NULL;

    arq = fopen ("config.txt", "r"); // para leitura

    int a = 0;

    int t_arq_number = 0;
    int idx = 0;
    int radius_length = 0;
    int chord_length = 0;
    int twist_length = 0;

    if (arq == NULL){
        printf("ERRO AO ABRIR O ARQUIVO");
        return d;
    }

    while (fgets(l, sizeof(l), arq) != NULL) {

        if (strncmp(l, rho, strlen(rho))==0){
            content = strchr(l, ':');
            remover_dois_pontos(content);
            rho2 = strtof(content, NULL);
            printf("rho: %f", rho2);
        }

        if (strncmp(l, mu, strlen(mu))==0){
            content = strchr(l, ':');
            remover_dois_pontos(content);
            mu2 = strtof(content, NULL);
            printf("\nmu: %f", mu2);
        }

        if (strncmp(l, R, strlen(R))==0){
            content = strchr(l, ':');
            remover_dois_pontos(content);
            R2 = strtof(content, NULL);
            printf("\nR: %f", R2);
        }

        if (strncmp(l, B, strlen(B))==0){
            content = strchr(l, ':');
            remover_dois_pontos(content);
            B2 = strtof(content, NULL);
            printf("\nB: %f", B2);
        }

        if (strncmp(l, AIRFOIL_NUMBER, strlen(AIRFOIL_NUMBER))==0){
            content = strchr(l, ':');
            remover_dois_pontos(content);
            AIRFOIL_NUMBER2 = strtof(content, NULL);
            airfoils2 = malloc((int)AIRFOIL_NUMBER2*sizeof(Airfoil));
            printf("\nAIRFOIL NUMBER: %f", AIRFOIL_NUMBER2);
        }

        if (strncmp(l, ARQ_NUMBER, strlen(ARQ_NUMBER))==0){
            content = strchr(l, ':');
            remover_dois_pontos(content);
            ARQ_NUMBER2 = strtof(content, NULL);
            for (int i = 0; i < (int)AIRFOIL_NUMBER2; i++){
                airfoils2[i].arq_names = malloc((int)ARQ_NUMBER2*sizeof(char *));

                for (int j = 0; j < ARQ_NUMBER2; j++) {
                    airfoils2[i].arq_names[j] = malloc(100 * sizeof(char));
                }
            }

            printf("\nARQ_NUMBER: %f", ARQ_NUMBER2);

        }

        char transient[100] = "";
        if (strncmp(l, chord, strlen(chord))==0){
            content = strchr(l, ':');
            remover_dois_pontos(content);
            int j = 0;
            int k = 0;
            for (int i = 0; content[i] != '\0'; i++){
                if (content[i] != ' ' && content[i] != '\n'){
                    transient[k] = content[i];
                    k = k + 1;
                    transient[k] = '\0';
                } else {
                    chord2[j] = strtof(transient, NULL)/1000;
                    transient[0] = '\0';
                    j = j + 1;
                    k = 0;
                }
            }

            chord2 = realloc(chord2, j*sizeof(double));
            chord_length = j;
            printf("\n");
            printf("chord: ");
            for (int n = 0; n < j; n++){
                printf("%f ", chord2[n]);
            }
        }

        transient[0] = '\0';
        if (strncmp(l, radius, strlen(radius))==0){
            content = strchr(l, ':');
            remover_dois_pontos(content);
            int j = 0;
            int k = 0;
            for (int i = 0; content[i] != '\0'; i++){

                if (content[i] != ' ' && content[i] != '\n'){
                    transient[k] = content[i];
                    k = k + 1;
                    transient[k] = '\0';
                } else {
                    radius2[j] = strtof(transient, NULL);
                    transient[0] = '\0';
                    j = j + 1;
                    k = 0;
                }
            }
            radius2 = realloc(radius2, j*sizeof(double));
            radius_length = j;
            printf("\n");
            printf("radius: ");
            for (int n = 0; n < j; n++){
                printf("%f ", radius2[n]);
            }
        }

        transient[0] = '\0';
        if (strncmp(l, twist, strlen(twist))==0){
            content = strchr(l, ':');
            remover_dois_pontos(content);
            int j = 0;
            int k = 0;
            for (int i = 0; content[i] != '\0'; i++){
                if (content[i] != ' ' && content[i] != '\n'){
                    transient[k] = content[i];
                    k = k + 1;
                    transient[k] = '\0';
                } else {
                    twist2[j] = strtof(transient, NULL)*M_PI/180;
                    transient[0] = '\0';
                    j = j + 1;
                    k = 0;
                }
            }
            twist2 = realloc(twist2, j*sizeof(double));
            twist_length = j;
            printf("\n");
            printf("twist: ");
            for (int n = 0; n < j; n++){
                printf("%f ", twist2[n]);
            }
        }


        //Airfoil airfoils[(int)AIRFOIL_NUMBER2];

        char *transient2 = malloc(100*sizeof(char));

        int save_i = 0;

        if (strncmp(l, airfoil_parser, strlen(airfoil_parser))==0){
            content = strchr(l, ':');
            remover_dois_pontos(content);


            // CHECA CHECA
            if (content == NULL){
                printf("CONTENT NULL");
            } else {
                //for (int i = 0; content[i] != '\0'; i++){
                //printf("%c", content[i]);
                //}
            }


            for (int i = 0; content[i] != '/'; i++){
                airfoils2[a].name[i] = content[i];
                airfoils2[a].name[i+1] = '\0';
                save_i = i;
            }


            save_i = save_i + 2;
            int k = 0;
            int j = 0;
            int save_i2 = 0;
            for (int i = save_i; content[i] != '-'; i++){
                transient2[k] = content[i];
                k = k+1;
                transient2[k] = '\0';
                j = i;
                save_i2 = i;
            }

            //printf("\nTESTE TRANSIENT: %s", transient2);
            transient2 = realloc(transient2, (j - save_i + 1)*sizeof(char));

            //printf("\nTESTE 2 TRANSIENT: %s", transient2);
            airfoils2[a].start = strtof(transient2, NULL);

            k = 0;
            j = 0;
            save_i2 = save_i2 + 2;
            transient2 = realloc(transient2, 100*sizeof(char));
            transient2[0] = '\0';
            for (int i = save_i2; content[i] != '\0' && content[i] != '\n'; i++){
                transient2[k] = content[i];
                k = k+1;
                transient2[k] = '\0';
                j = i;
            }
            transient2 = realloc(transient2, (j - save_i2 + 1)*sizeof(char));
            airfoils2[a].stop = strtof(transient2, NULL);

            //TESTE
            //printf("\nAirfoil name: %s", airfoils2[a].name);
            //printf("\nAirfoil start: %f", airfoils2[a].start);
            //printf("\nAirfoil end: %f", airfoils2[a].stop);

            a = a + 1;
        }



        if (strncmp(l, XFOIL, strlen(XFOIL))==0){

            content = strchr(l, ':');
            remover_dois_pontos(content);

            transient[0] = '\0';
            int s_i = 0;
            for (int i = 0; content[i] != '/'; i++){
                transient[i] = content[i];
                transient[i+1] = '\0';
                s_i = i;
            }

            // CHECA
            //printf("\ntransient %s", transient);

                if (strcmp(airfoils2[idx].name, transient)==0){

                    // CHECA
                    // printf("\nEntrou no if");

                    s_i = s_i+2;

                    char transient2[100] = "";
                    int aux = 0;
                    for (int j = s_i; content[j] != '\0' && content[j] != '\n'; j++){

                        transient2[aux] = content[j];
                        aux = aux + 1;
                        transient2[aux] = '\0';

                    }
                    strcpy(airfoils2[idx].arq_names[t_arq_number], transient2);
                    printf("\nAIRFOIL[%d] | ARQ_NAME[%d] = %s", idx, t_arq_number, airfoils2[idx].arq_names[t_arq_number]);
                    t_arq_number = t_arq_number + 1;

                    // CHECA 
                    //printf("\ntransient2 %s", transient2);

                }

                if (t_arq_number >= ARQ_NUMBER2){
                    t_arq_number = 0;
                    if (AIRFOIL_NUMBER2 - 1 > idx) {
                        idx = idx + 1;
                    }
                }
        }

    }

    fclose(arq);


    //TESTE TESTE
    //
    //for (int i = 0; i < AIRFOIL_NUMBER2; i++){   
    //    for (int z=0; z < ARQ_NUMBER2; z++){
    //       printf("\n AIRFOIL %d name %s arq_number %d arq_name %s", i, airfoils2[i].name, z, airfoils2[i].arq_names[z]); 
    //    }
    //}

    d.radius_length = radius_length;
    d.chord_length = chord_length;
    d.twist_length = twist_length;
    d.rho = rho2;
    d.mu = mu2;
    d.R = R2;
    d.B = B2;
    d.AIRFOIL_NUMBER = AIRFOIL_NUMBER2;
    d.ARQ_NUMBER = ARQ_NUMBER2;
    d.chord = chord2;
    d.radius = radius2;
    d.twist = twist2;
    d.airfoils = airfoils2;

    return d;
}



//int main() {

//    parser();
//    return 0;
//}
