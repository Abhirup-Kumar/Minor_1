#include <stdio.h>
#include <stdlib.h>
#include "huffman.h"

// Encoding Function Declaration
int encode(float *,float *, float *);

int main(){

	float a =0.0;
	float d=0.0;
	float u=0.0;
	//Calling huffman function
	encode(&a,&d,&u);
	
	printf("Orignial filesize: %f \nCompressed filesize: %f\nDecompressed file size:%f\n", a, d,u);

	return 0;
	
	}
	
	
int encode(float *a,float *d,float *u){
	// Reading the BMP File
    int i, j;
	char filename[] = "file.bmp"; 
	int data = 0, offset, bpp = 0, width, height;
	long bmpsize = 0, bmpdataoff = 0;
	int** image;
	int temp = 0;

	
	FILE* image_file,*fp1;
	fp1= fopen("file.bmp", "rb");if (fp1== NULL){printf("Error Opening File!!");exit(0);}
	char c;

	image_file = fopen(filename, "rb");
	if (image_file == NULL)
	{
		printf("Error Opening File!!");
		exit(1);
	}
	else
	{

		
		// Set file position of the stream to the beginning
        offset = 0;
		
		// Set offset to 2 
		offset = 2;

		fseek(image_file, offset, SEEK_SET);

		// Getting size of BMP File
		fread(&bmpsize, 4, 1, image_file);
		offset = 10;

		fseek(image_file, offset, SEEK_SET);

		// Bitmap data offset
		fread(&bmpdataoff, 4, 1, image_file);

		// Getting height and width of the image
		fseek(image_file, 18, SEEK_SET);

		fread(&width, 4, 1, image_file);

		fread(&height, 4, 1, image_file);

		// Number of bits per pixel
		fseek(image_file, 2, SEEK_CUR);

		fread(&bpp, 2, 1, image_file);

		// Setting offset to start of pixel data
		fseek(image_file, bmpdataoff, SEEK_SET);

		// Creating Image array
		image = (int**)malloc(height * sizeof(int*));

		for (i = 0; i < height; i++)
		{
			image[i] = (int*)malloc(width * sizeof(int));
		}

		
		// Reading the BMP File
		// into Image Array
		int numbytes = (bmpsize - bmpdataoff) / 3;
		for (i = 0; i < height; i++)
		{
			for (j = 0; j < width; j++)
			{
				fread(&temp, 3, 1, image_file);
				temp = temp & 0x0000FF;
				image[i][j] = temp;
			}
		}
	}

	// Finding the probability
	// of occurrence
	int hist[256];
	for (i = 0; i < 256; i++)
		hist[i] = 0;
	for (i = 0; i < height; i++)
		for (j = 0; j < width; j++)
			hist[image[i][j]] += 1;

	// Finding number of
	// non-zero occurrences
	int nodes = 0;
	for (i = 0; i < 256; i++)
		if (hist[i] != 0)
			nodes += 1;

	// Calculating minimum probability
	float p = 1.0, ptemp;
	for (i = 0; i < 256; i++)
	{
		ptemp = (hist[i] / (float)(height * width));
		if (ptemp > 0 && ptemp <= p)
			p = ptemp;
	}


	// Calculating max length
	// of code word
	i = 0;
	while ((1 / p) > fib(i))
		i++;
	int maxcodelen = i - 3;

	// Defining Structures pixfreq
	struct pixfreq
	{
		int pix, larrloc, rarrloc;
		float freq;
		struct pixfreq *left, *right;
		char code[maxcodelen];
	};

	// Defining Structures
	// huffcode
	struct huffcode
	{
		int pix, arrloc;
		float freq;
	};

	// Declaring structs
	struct pixfreq* pix_freq;
	struct huffcode* huffcodes;
	int totalnodes = 2 * nodes - 1;
	pix_freq = (struct pixfreq*)malloc(sizeof(struct pixfreq) * totalnodes);
	huffcodes = (struct huffcode*)malloc(sizeof(struct huffcode) * nodes);
	
	// Initializing
	
	j = 0;
	int totpix = height * width;
	float tempprob;
	for (i = 0; i < 256; i++)
	{
		if (hist[i] != 0)
		{

			//storing pixel intensity value
			huffcodes[j].pix = i;
			pix_freq[j].pix = i;

			// location of the node is stored in the pix_freq array
			huffcodes[j].arrloc = j;

			// probability of occurrence
			tempprob = (float)hist[i] / (float)totpix;
			pix_freq[j].freq = tempprob;
			huffcodes[j].freq = tempprob;

			// Declaring the child of leaf
			// node as NULL pointer
			pix_freq[j].left = NULL;
			pix_freq[j].right = NULL;

			// initializing the code
			// word as end of line
			pix_freq[j].code[0] = '\0';
			j++;
		}
	}

	// Sorting the histogram
	struct huffcode temphuff;

	// Sorting w.r.t probability of occurence of each pixel intensity
	for (i = 0; i < nodes; i++)
	{
		for (j = i + 1; j < nodes; j++)
		{
			if (huffcodes[i].freq < huffcodes[j].freq)
			{
				temphuff = huffcodes[i];
				huffcodes[i] = huffcodes[j];
				huffcodes[j] = temphuff;
			}
		}
	}

	// Building Huffman Tree
	float sumprob;
	int sumpix;
	int n = 0, k = 0;
	int nextnode = nodes;

	//total number of nodes in huffman tree is (2* number of nodes)-1
	while (n < nodes - 1)
	{

		// Adding the lowest two probabilities
		sumprob = huffcodes[nodes - n - 1].freq + huffcodes[nodes - n - 2].freq;
		sumpix = huffcodes[nodes - n - 1].pix + huffcodes[nodes - n - 2].pix;

		// Appending to the pix_freq Array
		pix_freq[nextnode].pix = sumpix;
		pix_freq[nextnode].freq = sumprob;
		pix_freq[nextnode].left = &pix_freq[huffcodes[nodes - n - 2].arrloc];
		pix_freq[nextnode].right = &pix_freq[huffcodes[nodes - n - 1].arrloc];
		pix_freq[nextnode].code[0] = '\0';
		i = 0;

		// sorting and updating the huffcodes array simultaneously for obtaining new position for the combined node
		while (sumprob <= huffcodes[i].freq)
			i++;

		// Inserting the new node in the huffcode array
		for (k = nodes; k >= 0; k--)
		{
			if (k == i)
			{
				huffcodes[k].pix = sumpix;
				huffcodes[k].freq = sumprob;
				huffcodes[k].arrloc = nextnode;
			}
			else if (k > i)

				
				huffcodes[k] = huffcodes[k - 1];

		}
		n += 1;
		nextnode += 1;
	}

	// Assigning huff Code through traversing to root node
	
	char left = '0';
	char right = '1';
	int index;
	for (i = totalnodes - 1; i >= nodes; i--)
	{
		if (pix_freq[i].left != NULL)
			strconcat(pix_freq[i].left->code, pix_freq[i].code, left);
		if (pix_freq[i].right != NULL)
			strconcat(pix_freq[i].right->code, pix_freq[i].code, right);
	}

	// Encoding the image into a text file
	int pix_val;
	int l;

	// Writing the Huffman encoded image into a text file created using file handling
	FILE* imagehuff = fopen("encoded_image.txt", "wb");
	for (i = 0; i < height; i++)
		for (j = 0; j < width; j++)
		{
			pix_val = image[i][j];
			for (l = 0; l < nodes; l++)
				if (pix_val == pix_freq[l].pix)
					fprintf(imagehuff, "%s", pix_freq[l].code);
		}

	// Printing Huffman Codes
	printf("Huffmann Codes:\n");
	printf("Pixel Intensity Values - ASCII Code\n");
	for (i = 0; i < nodes; i++) {
		if (snprintf(NULL, 0, "%d", pix_freq[i].pix) == 2)
			printf("%d	-> %s\n", pix_freq[i].pix, pix_freq[i].code);
		else
			printf("%d	-> %s\n", pix_freq[i].pix, pix_freq[i].code);
	}

	// Calculating Average Bit Length
	float avgbitnum = 0;
	for (i = 0; i < nodes; i++)
		avgbitnum += pix_freq[i].freq * codelen(pix_freq[i].code);
	
	
	//Calculating actual image size
	int actual_size_bit=0;
	actual_size_bit=8*(height*width);
	float actual_size=0;
	actual_size = (float)(actual_size_bit)*0.000125;  
	
	printf("\nThere are total %d unique pixel intensities in the image\n", nodes); //Unique pixel intensity value in the image file
	
		
		
		
		
	//Calculating difference between actual and compressed file
    int tsize_bit=0;
	tsize_bit=nodes * avgbitnum;
	float tsize=(float)(tsize_bit)*0.000125;
	float diff=0;
	diff=actual_size-tsize;
	
	
	
	//Initializing file pointer for uncompressed file
	FILE* fp2;
	fp2= fopen("uncompressed.bmp", "wb");if (fp2 == NULL) { printf("Cannot open file\n"); exit(0); } 
	
	c = fgetc(fp1); 
    while (c != EOF) 
    { 
        fputc(c, fp2); 
        c = fgetc(fp1); 
    } 
  



//calculating decompresses file size
char ch;
    int size = 0;
    char * arrr = "compressed.bmp";
    fp2 = fopen(arrr, "rb");
    if (fp2 == NULL)
        printf("\n");
    else
        printf("\nFile opened ");
    fseek(fp2, 0, 2);    /* file pointer at the end of file */
	int m_factor=2;
    size = ftell(fp2);   /* take a position of file pointer un size variable */
	float u_size=0.0;
	u_size= (float)(size)*(0.125/m_factor);
	fclose(fp1); 
    fclose(fp2);
	
	
	
	//Assigning required value to pointers a,d and u
    *a=actual_size;
    *d=diff;
	*u= u_size;

    return 0;
	
	
}
	
   
	
	