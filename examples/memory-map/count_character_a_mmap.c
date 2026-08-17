#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char **argv) {
    if (argc < 2 || argc > 3) {
        fprintf(stderr, "Usage: %s <input-file> [character]\n", argv[0]);
        return EXIT_FAILURE;
    }
    const unsigned char target = (unsigned char)(argc == 3 ? argv[2][0] : 'a');
    const int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        perror(argv[1]);
        return EXIT_FAILURE;
    }
    struct stat info;
    if (fstat(fd, &info) == -1) {
        perror("fstat");
        close(fd);
        return EXIT_FAILURE;
    }
    if (info.st_size == 0) {
        close(fd);
        printf("Character '%c' occurs 0 times\n", target);
        return EXIT_SUCCESS;
    }
    const size_t size = (size_t)info.st_size;
    const unsigned char *contents = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (contents == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return EXIT_FAILURE;
    }
    close(fd);
    unsigned long long count = 0;
    for (size_t index = 0; index < size; ++index)
        if (contents[index] == target)
            ++count;
    if (munmap((void *)contents, size) == -1) {
        perror("munmap");
        return EXIT_FAILURE;
    }
    printf("Character '%c' occurs %llu times\n", target, count);
    return EXIT_SUCCESS;
}
