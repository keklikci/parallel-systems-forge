#include <cstdlib>
#include <fstream>
#include <iostream>

int main(int argc, char **argv) {
    if (argc < 2 || argc > 3) {
        std::cerr << "Usage: " << argv[0] << " <input-file> [character]\n";
        return EXIT_FAILURE;
    }
    const char target = argc == 3 ? argv[2][0] : 'a';
    std::ifstream file(argv[1], std::ios::binary);
    if (!file) {
        std::cerr << "Unable to open input file: " << argv[1] << '\n';
        return EXIT_FAILURE;
    }
    unsigned long long count = 0;
    char value;
    while (file.get(value))
        if (value == target)
            ++count;
    if (!file.eof()) {
        std::cerr << "Unable to read input file: " << argv[1] << '\n';
        return EXIT_FAILURE;
    }
    std::cout << "Character '" << target << "' occurs " << count << " times\n";
    return EXIT_SUCCESS;
}
