#include "huff.h"

long int filesize(FILE *fp)
{
	fseek(fp, 0L, SEEK_END); 

    // calculating the size of the file 
    long int res = ftell(fp);
	return res;
}

int main(int argv, char** argc)
{
	if(argv != 4)
	{
		printf("Uso: %s [c/d] input output\n", argc[0]);
		return 0;
	}
	
	FILE * in, * out;
	
	if(!(in = fopen(argc[2], "rb"))) 
	{
		perror("ERRROR opening input file");
		return -1;
	}
	if(!(out = fopen(argc[3], "wb")))
	{
		perror("ERROR opening output file");
		return -1;
	}
	
	if(*argc[1] == 'c')
		huff_cod(in, out);
	else if(*argc[1] == 'd')
		huff_dec(in, out);
	else
		printf("Uso: %s [c/d] input output\n", argc[0]);
	
	long int insize = filesize(in);
	long int outsize = filesize(out);
	if(*argc[1] == 'c')
		printf("\nFile size of orginal file and compressed file in bytes is:  %li %li", insize, outsize);
	else if(*argc[1] == 'd')
		printf("\nFile size of compressed file and decompressed file in bytes is:  %li %li", insize, outsize);
	
	fclose(in);
	fclose(out);
	return 0;
}