#include "stdio.h"
#include "stdlib.h"
#include "math1.h"

int main(int argc, char *argv[]){
    // Declaring a pointer to realisation of an integral function
    float (*ptr_to_fun)(float a, float b, float n);
    ptr_to_fun = &trapez_integral;

    // Argument error check
    if (argc == 1){
        perror("INCORRECT INPUT KEYS (at least 3 arguments needed)\n");
        exit(0);
    }
    else {
        // Argument error check
        if ((*argv[1] == '1') & (((argc - 2) % 3) != 0)) {
            perror("INCORRECT INPUT KEYS (for every computation of integral 3 args needed)\n");
            exit(0);
        }
        // Answer printing
        else{
            int n = 2;
            for (int i = 0; i < ((argc - 2) / 3); i++){
                printf("%f\n", ptr_to_fun(atoi(argv[n]), atoi(argv[n+1]), atoi(argv[n+2])));
                n += 3;
            }
        }
        return 0;
    }
}
