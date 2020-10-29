#include<stdio.h>
#include<sys/stat.h>
#include<stdlib.h>
#include"Huffman.h"

void printCharWithFreq(char *str) // freq part
{
	int n = 0, jump=0;
	char *newstr = str;

	while(*(newstr+n) != '\0')
	{
		n++;
	}

	int freq[256]={0};

	// freq of each character
	for (int i = 0; i < n; i++)
		freq[*(newstr+i)]++;

	for (int i = 0; i < n; i++) 
	{
		// frequency not equal to 0
		if (freq[*(newstr+i)] != 0)
		{
			printf("%c ->", *(newstr+i) ); //char
			printf(" %d  " , freq[*(newstr+i)] ); //freq
			
			if(jump%4 == 0)
			printf("\n");

			jump++;
			freq[*(newstr+i)] = 0; //freq=0, to not repeat printed char
		}
	}
}


int main()
{
char *FileName = "tes.txt";

char *buf = NULL;
FILE *f = NULL;
struct stat stats;

f = fopen(FileName, "r");		//file part
if (NULL == f)
    printf("Unable to open the file %s: %s\n", FileName, strerror(errno));
else
{
    // get file info (we need the size)
    fstat(fileno(f), &stats);
    // allocate buffer for file content and 1 extra byte for the NULL terminator
    buf = (char *)malloc(stats.st_size + 1);
    if (NULL == buf)
        printf("Out of memory\n");
    else
    {
        // store(read)file content into buffer
        fread(buf, 1, stats.st_size, f);
        buf[stats.st_size] = 0; // appending last as null
    }
    // close file
    fclose(f);
}

if (NULL != buf)
{
    printCharWithFreq(buf);
    free(buf);
}
return 0;
}

