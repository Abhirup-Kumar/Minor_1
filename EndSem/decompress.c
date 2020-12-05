#include<stdio.h>
#include<stdlib.h>
float dd=0.0;
long int findSize(char file_name[]) 
{ 
    // opening the file in read mode 
    FILE* fp = fopen(file_name, "r"); 
  
    // checking if the file exist or not 
    if (fp == NULL) { 
        printf("File Not Found!\n"); 
        return -1; 
    } 
  
    fseek(fp, 0L, SEEK_END); 
  
    // calculating the size of the file 
    long int res = ftell(fp); 
  
    // closing the file 
    fclose(fp); 
  
    return res; 
} 
  
int decode(float *dd){
	//Initializing file pointer for uncompressed file
	FILE* fp1;
	FILE* fp2;
	char c;
	fp1= fopen("file.bmp", "rb");if (fp1== NULL){printf("Error Opening File!!");exit(0);}
	fp2= fopen("uncompressed.bmp", "wb");if (fp2 == NULL) { printf("Cannot open file\n"); exit(0); } 
	
	c = fgetc(fp1); 
    while (c != EOF) 
    { 
        fputc(c, fp2); 
        c = fgetc(fp1); 
    } 
  
char file_name[] = { "uncompressed.bmp" }; 
    long int res = findSize(file_name); 
    if (res != -1) 
       printf("Size of the file is %ld bytes \n", res);

float u_size=(float)res*0.00128;
//u_size=2.0;



*dd=u_size;

    return 0;
	}
	
	int main()
	{
		decode(&dd);
		printf("%f",dd);
		
	}
	
	
