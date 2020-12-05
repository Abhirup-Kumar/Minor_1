#include<stdio.h>
#include "huff.h"
#include<string.h>
#define MAX 60

int filesize(FILE *fp)
{
	fseek(fp, 0L, SEEK_END); 

    // calculating the size of the file 
    int res = ftell(fp);
	return res;
}

int dohuffman(char option)
{	
    FILE *in, *out;
	
	
	if(option == 'c')
	{
		char *infilename = "/home/dev/Documents/Finale/src/tex.txt";	
		char *outfilename = "/home/dev/Documents/Finale/src/com.txt";
		if(!(in = fopen(infilename, "rb"))) 
	{
		perror("ERRROR opening input file");
		return -1;
	}

	if(!(out = fopen(outfilename, "wb")))
	{
		perror("ERROR opening output file");
		return -1;
	}
	}

	if(option == 'd')
	{
		char *infilename = "/home/dev/Documents/Finale/src/com.txt";	
		char *outfilename = "/home/dev/Documents/Finale/src/decom.txt";
		if(!(in = fopen(infilename, "rb"))) 
	{
		perror("ERRROR opening input file");
		return -1;
	}
	if(!(out = fopen(outfilename, "wb")))
	{
		perror("ERROR opening output file");
		return -1;
	}
	}


	if(option == 'c')
		huff_cod(in, out);
	else if(option == 'd')
		huff_dec(in, out);
	else
		printf("Provide option to either compress or decompress! \n");
	
	int insize = filesize(in);
	int outsize = filesize(out);

	fclose(in);
	fclose(out);
	insize /= 1024;
	return insize;
}
