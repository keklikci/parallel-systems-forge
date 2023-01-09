#include <iostream>
#include <fstream>

using namespace std;

int main() {

    // string line;
    ifstream filename("loremipsum.txt");
    int counter = 0;
    char chr;
    while(filename >> chr) {
        if(chr == 'a') {
            counter++;
        }
    }
    filename.close();
    printf("Character 'a' occurs %d times\n", counter); 
    // Character 'a' occurs 19082160 times
    return 0;
}