#include <stdio.h>
#include <stdlib.h>

int main() {

   FILE *fp = fopen("loremipsum.txt", "r"); 
   char chr;
   int counter = 0;
   if(fp == NULL) {
       printf("File could not be opened, exiting...\n");
       return 0;
   }
   while((chr = fgetc(fp)) != EOF) {
       if(chr == 'a') {
           counter += 1;
       }
    }
   printf("Character 'a' occurs %d times\n", counter); 
   // Character 'a' occurs 19082160 times
   fclose(fp);
   return 0;
}
