#include "unistd.h"
#include "stdio.h"
#include <stdlib.h>

#include "string.h"

#include "sys/stat.h"
#include "sys/types.h"
#include "errno.h"
#include "fcntl.h"


unsigned int time(void *pVoid);

int main(){
    int fd1[2];
    if (pipe(fd1) == -1){
        printf("An error ocurred with opening the pipe\n");
    }


    int id = fork();

    if (id < 0){
        perror("An error occurred with fork");
        return -1;
    }
    
    else if (id > 0){
        char s[20];
        printf("Enter the filename:\n");
        scanf("%s",s);
        int len = strlen(s);
        close(fd1[0]);
        write(fd1[1], &len, sizeof(int));
        write(fd1[1], s, sizeof(int) * len);
        close(fd1[1]);

        if (mkfifo("myfifo", 0777) == -1) {
            if (errno != EEXIST) {
                printf("Couldnt create fifo file");
                return 1;
            }
        }

        int fd = open("myfifo", O_WRONLY);

        srand(time(NULL));

        int n = rand() % 10 + 1;
        
        if(write(fd, &n, sizeof(int)) == -1){
            return 2;
        }
        

        printf("Enter %d different digits\n", n);

        float data[11];
        for(int i = 0; i < n; i++){
            scanf("%f", &data[i]);
            if(write(fd, &data[i], sizeof(float)) == -1){
                return 3;
            }
        }
        close(fd);
    }
    else{
        int len;
        char s[20];
        
        read(fd1[0], &len, sizeof(int));
        read(fd1[0], s, sizeof(char) * len);
        close(fd1[1]);
        close(fd1[1]);
        
        char *args[] = {"./child", s, NULL};
        execvp(args[0], args);
    }
    return 0;
}
