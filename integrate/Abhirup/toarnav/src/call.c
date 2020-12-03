#include"main.h"
#include<stdio.h>

int main()
{   long int originalsize = 0, compressedsize = 0;
    originalsize = dohuffman('c');
    compressedsize = dohuffman('d');
    printf("orignial filesize: %li \nCompressed filesize: %li\n", originalsize, compressedsize);
    return 0; // size is in bytes.
}