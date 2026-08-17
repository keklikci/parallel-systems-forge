#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    if (argc < 2 || argc > 3) {
        fprintf(stderr, "Usage: %s <input-file> [character]\n", argv[0]);
        return EXIT_FAILURE;
    }
    const unsigned char target = (unsigned char)(argc == 3 ? argv[2][0] : 'a');
    FILE *file = fopen(argv[1], "rb");
    if (file == NULL) {
        perror(argv[1]);
        return EXIT_FAILURE;
    }
    unsigned long long count = 0;
    int value;
    while ((value = fgetc(file)) != EOF) {
        if ((unsigned char)value == target) ++count;
    }
    if (ferror(file) != 0 || fclose(file) != 0) {
        perror("reading input file");
        return EXIT_FAILURE;
    }
    printf("Character '%c' occurs %llu times\n", target, count);
    return EXIT_SUCCESS;
}
