#include<stdio.h>
#include "huff.h"
#include<string.h>
#define MAX 60

long int filesize(FILE *fp)
{
	fseek(fp, 0L, SEEK_END); 

    // calculating the size of the file 
    long int res = ftell(fp);
	return res;
}

int dohuffman(char option)
{	
    FILE *in, *out;
	


    // printf("Enter the input file name: ");
    // fgets(infilename,MAX,stdin);
    // printf("Enter the output compressed file name: ");
    // fgets(outfilename,MAX,stdin);
    // int i=0, j=0;
	// char *absolute = "/home/dev/Documents/huffman-integrate/src/1";
	// char newabsolute[100];
	// while(*(absolute+i) != '\0')
	// 	{
	// 		j++;
	// 	}
	// 	i=0;
	// //printf("%d", i);
	// printf("Enter the input file name: ");
    // fgets(newabsolute,MAX,stdin);
	// while(newabsolute[i]!= '\0')
	// 	{
	// 		*(absolute+j) = newabsolute[i];
	// 		i++;
	// 	}
	// *absolute = '\0';

	// char *buffer = ( char * ) malloc( strlen( line ) + 1 );
	// buffer[0] = '\0';
	// char *p = buffer;
	// for ( size_t i = 0; i < strlen( line ); i++ )
	// {
    // *p++ = line[i];
    // *p = '\0';
	// }
	
	if(option == 'c')
	{
		// char *infilename;
		// printf("Enter the input file name: ");
        // fgets(infilename,MAX,stdin);
		char *infilename = "/home/dev/Documents/huffman-integrate/src/tex.txt";	
		char *outfilename = "/home/dev/Documents/huffman-integrate/src/com.txt";
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
		char *infilename = "/home/dev/Documents/huffman-integrate/src/com.txt";	
		char *outfilename = "/home/dev/Documents/huffman-integrate/src/decom.txt";
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
	
	long int insize = filesize(in);
	long int outsize = filesize(out);
	if(option == 'c')
		printf("\nFile size of orginal file and compressed file in bytes is:  %li %li", insize, outsize);
	else if(option == 'd')
		printf("\nFile size of compressed file and decompressed file in bytes is:  %li %li", insize, outsize);
	
	fclose(in);
	fclose(out);
	return 0;
}