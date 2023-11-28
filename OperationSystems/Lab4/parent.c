#include "unistd.h"
#include "stdio.h"
#include <stdlib.h>
#include "string.h"
#include <sys/mman.h>
#include "sys/stat.h"
#include "sys/types.h"
#include "errno.h"
#include "fcntl.h"


unsigned int time(void *pVoid);

int main(){

    int fd = open("data.txt", O_RDONLY);
    struct stat sb;

    if(fstat(fd, &sb) == -1){
        perror("couldn't get the file size\n");
    }

    int id = fork();

    if (id < 0){
        perror("An error occurred with fork");
        return -1;
    }

    else if (id > 0) {

        FILE *fp;
        fp = fopen("data.txt", "w");

        char s[30];
        printf("Enter the filename:\n");
        scanf("%s", s);

        int len = strlen(s);         // Длина строки

        srand(time(NULL));
        int n = rand() % 9 + 1;

        fprintf(fp, "%d\n", len);
        fprintf(fp, "%s\n", s);
        fprintf(fp, "%d\n", n);


        printf("Enter %d different digits\n", n);

        float x;
        for (int i = 0; i < n; i++) {
            scanf("%f", &x);
            fprintf(fp, "%.2f\n", x);
        }
        fclose(fp);

        char *in = mmap(NULL, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);
        printf("%p я родитель\n", in);
        close(fd);

        pid_t wpid;
        int status = 0;
        while ((wpid = wait(&status)) > 0);

    }
    else{
        sleep(10);

        char *out = mmap(NULL, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);
        printf("%p я ребенок\n", out);

        int n_of_chars = out[0] - '0';
        int n_of_digits = out[n_of_chars+3] - '0';

        char str[n_of_chars];

        for (int i = 2; i < 2 + n_of_chars; i++){
            str[i-2] = out[i];
        }
        float sum = 0;
        int start = 5 + n_of_chars;
        for(int j = 0; j < n_of_digits; j++) {
            int i = 0;
            char a[4];
            while (i < 4) {
                a[i] = out[start];
                i += 1;
                start += 1;
            }
            sum += atof(a);
            start += 1;

        }

        FILE *fptr2;
        fptr2 = fopen(str,"w");
        fprintf(fptr2,"The answer is: %.2f\n", sum);
        fclose(fptr2);
        close(fd);
    }
    return 0;
}
