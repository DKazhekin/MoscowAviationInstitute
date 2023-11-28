#include "unistd.h"
#include "stdio.h"
#include <stdlib.h>

#include "string.h"
#include "sys/stat.h"
#include "sys/types.h"
#include "errno.h"
#include "fcntl.h"


int main(int argc, char* argv[]){
    int n;
    float arr[11];
    
    int fd = open("myfifo", O_RDONLY);
    
    if (fd == -1){
        return 1;
    }
    
    if(read(fd, &n, sizeof(int)) == -1){
        return 2;
    };
        
    for(int i = 0; i < n; i++){
        if (read(fd, &arr[i], sizeof(float)) == -1){
            return 3;
        }
    }
    
    float sum = 0;
    
    for(int i = 0; i < n; i++){
        sum += arr[i];
    }
    
    FILE *fptr;
    fptr = fopen(argv[1],"w");
    
    fprintf(fptr,"The answer is: %.2f\n", sum);
    fclose(fptr);
    
    close(fd);
    return 0;
}
