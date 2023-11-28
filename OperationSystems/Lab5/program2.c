#include "stdio.h"
#include "stdlib.h"
#include "math1.h"
#include "math2.h"

int main(int argc, char *argv[]){
    // Argument error check
    if (argc == 1){
        perror("INCORRECT INPUT KEYS (need more arguments)\n");
        exit(0);
    }
    else {
        // Argument error check
        if (*argv[1] == '1') {
            if ((*argv[1] == '1') & (((argc - 2) % 3) != 0)) {
                perror("INCORRECT INPUT KEYS (for every computation of integral 3 args needed)\n");
                exit(0);
            }
            else {
                int check = 1;
                int tmp = 1;

                // Declaring a pointer to realisation of an integral function
                float (*ptr_to_fun)(float a, float b, float n);

                printf("Current method - trapez\n");

                int n = 2;
                for (int i = 0; i < ((argc - 2) / 3); i++) {

                    printf("%s", "Enter '0' if you want to switch, '1' otherwise\n");
                    scanf("%d", &tmp);

                    if (tmp == 0) {
                        if (check == 1) {
                            check = 0;
                        }
                        else {
                            check = 1;
                        }
                    }

                    if (check == 1) {
                        ptr_to_fun = &trapez_integral;
                        printf("%f\n", ptr_to_fun(atoi(argv[n]), atoi(argv[n + 1]), atoi(argv[n + 2])));
                        n += 3;
                        printf("Current method - trapez\n");
                    }
                    else {
                        ptr_to_fun = &rectangle_integral;
                        printf("%f\n", ptr_to_fun(atoi(argv[n]), atoi(argv[n + 1]), atoi(argv[n + 2])));
                        n += 3;
                        printf("Current method - rectangle\n");
                    }

                }
            }
        }

        else if (*argv[1] == '2') {

            int check = 1;
            int tmp = 1;

            int (*ptr_to_fun)(long a);

            printf("Current translation - to binary\n");

            int n = 2;
            for (int i = 0; i < (argc - 2); i++) {

                printf("%s", "Enter '0' if you want to switch, '1' otherwise\n");
                scanf("%d", &tmp);

                if (tmp == 0) {
                    if (check == 1) {
                        check = 0;
                    }
                    else {
                        check = 1;
                    }
                }

                if (check == 1) {
                    ptr_to_fun = &dec2bin;
                    int k = atoi(argv[n]);
                    printf("%s ", argv[n]);
                    printf("%s", "----> ");
                    printf("%d\n", ptr_to_fun(k));
                    n += 1;
                    printf("Current method - to binary\n");
                }
                else {
                    ptr_to_fun = &dec2third;
                    int k = atoi(argv[n]);
                    printf("%s ", argv[n]);
                    printf("%s", "----> ");
                    printf("%d\n", ptr_to_fun(k));
                    n += 1;
                    printf("Current method - to ternary\n");
                }

            }
        }

        return 0;
    }
}
