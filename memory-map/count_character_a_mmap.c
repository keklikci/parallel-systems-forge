#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(){
    int fd = open ("loremipsum.txt", O_RDONLY); /* Open the file for reading. */
    struct stat s;
    int counter = 0;
    size_t size;
    int status = fstat(fd, &s);
    size = s.st_size;

    char *ptr = mmap(0,size,
            PROT_READ,MAP_PRIVATE,
            fd,0);
    if(ptr == MAP_FAILED){
        printf("Mapping Failed\n");
        return 1;
    }
    close(fd);
    int i;
    for(i = 0; i < size; i++) {
        if(ptr[i] == 'a') {
            counter += 1;
        }
    }
    status = munmap(ptr, size);
    if(status != 0){
        printf("Unmapping Failed\n");
        return 1;
    }
    printf("Character 'a' occurs %d times\n", counter); 
    // Character 'a' occurs 19082160 times
    return 0;
}
