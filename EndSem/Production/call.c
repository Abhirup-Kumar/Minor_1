#include"main.h"
#include<stdio.h>

int main()
{   int originalsize = 0, compressedsize = 0;
    originalsize = dohuffman('c');
    compressedsize = dohuffman('d');
    printf("orignial filesize in bytes: %d\nCompressed filesize in bytes: %d\n", originalsize, compressedsize);
    return 0; 
}
