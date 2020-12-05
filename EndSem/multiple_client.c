#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "main.h"
#include "dijak.h"
#include "huffman.h"

#define SIZE 1024


#define PORT 4444
int compressedsize = 0, decompressedsize = 0;
float aa=0.0;
int code(float *aa){


int i, j;
	char filename[] = "file.bmp";
	int data = 0, offset, bpp = 0, width, height;
	long bmpsize = 0, bmpdataoff = 0;
	int** image;
	int temp = 0;

	// Reading the BMP File
	FILE* image_file;

	image_file = fopen(filename, "rb");
	if (image_file == NULL)
	{
		printf("Error Opening File!!");
		exit(1);
	}
	else
	{

		// Set file position of the
		// stream to the beginning
		// Contains file signature
		// or ID "BM"
		offset = 0;

		// Set offset to 2, which
		// contains size of BMP File
		offset = 2;

		fseek(image_file, offset, SEEK_SET);

		// Getting size of BMP File
		fread(&bmpsize, 4, 1, image_file);

		// Getting offset where the
		// pixel array starts
		// Since the information is
		// at offset 10 from the start,
		// as given in BMP Header
		offset = 10;

		fseek(image_file, offset, SEEK_SET);

		// Bitmap data offset
		fread(&bmpdataoff, 4, 1, image_file);

		// Getting height and width of the image
		// Width is stored at offset 18 and
		// height at offset 22, each of 4 bytes
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

		// int image[height][width]
		// can also be done
		// Number of bytes in
		// the Image pixel array
		int numbytes = (bmpsize - bmpdataoff) / 3;

		// Reading the BMP File
		// into Image Array
		for (i = 0; i < height; i++)
		{
			for (j = 0; j < width; j++)
			{
				fread(&temp, 3, 1, image_file);

				// the Image is a
				// 24-bit BMP Image
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

			// pixel intensity value
			huffcodes[j].pix = i;
			pix_freq[j].pix = i;

			// location of the node
			// in the pix_freq array
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

	// Sorting w.r.t probability
	// of occurrence
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

	// Since total number of
	// nodes in Huffman Tree
	// is 2*nodes-1
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

		// Sorting and Updating the
		// huffcodes array simultaneously
		// New position of the combined node
		while (sumprob <= huffcodes[i].freq)
			i++;

		// Inserting the new node
		// in the huffcodes array
		for (k = nodes; k >= 0; k--)
		{
			if (k == i)
			{
				huffcodes[k].pix = sumpix;
				huffcodes[k].freq = sumprob;
				huffcodes[k].arrloc = nextnode;
			}
			else if (k > i)

				// Shifting the nodes below
				// the new node by 1
				// For inserting the new node
				// at the updated position k
				huffcodes[k] = huffcodes[k - 1];

		}
		n += 1;
		nextnode += 1;
	}

	// Assigning Code through
	// backtracking
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

	// Encode the Image
	int pix_val;
	int l;

	// Writing the Huffman encoded
	// Image into a text file
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
	/*printf("Huffmann Codes::\n\n");
	printf("pixel values -> Code\n\n");
	for (i = 0; i < nodes; i++) {
		if (snprintf(NULL, 0, "%d", pix_freq[i].pix) == 2)
			printf("	 %d	 -> %s\n", pix_freq[i].pix, pix_freq[i].code);
		else
			printf(" %d	 -> %s\n", pix_freq[i].pix, pix_freq[i].code);
	}*/

	// Calculating Average Bit Length
	float avgbitnum = 0;
	for (i = 0; i < nodes; i++)
		avgbitnum += pix_freq[i].freq * codelen(pix_freq[i].code);
	
	
	int actual_size_bit=0;
	actual_size_bit=8*(height*width);
	float actual_size=0;
	actual_size = (float)(actual_size_bit)*0.128;
	printf("The size of original file is: %f bytes\n",actual_size);
	printf("There are total %d unique pixel intensities in the image\n", nodes);
	
		//Calculating the total bits required
	
		
		
	//Function for bit difference
    int tsize_bit=0;
	tsize_bit=nodes * avgbitnum;
	float tsize=(float)(tsize_bit)*0.128;
	float diff=0;
	diff=actual_size-tsize;
	printf("The difference between the actual file and compresses file is: %f bytes\n",diff);	
*aa=tsize;	
}
typedef struct Something{
int source_IP[4];
int destination_IP[4];
}dumb;
dumb simulation(int choice,dumb pro)
{ 
	int ver;
	printf("Selecting random number of vertices to create a graph topology... \n \n");
	ver=vertex(15,10);
	printf("Number of vertices: ");
	printf("%d",ver);
	printf("\n \n");
	int **q;
	q=edges(10,1,ver);
	printf("Asigning wieghts to the edges between vertices... \n \n");
	/*printf("Matrix representation of the graph generated \n \n");
	disp(q,ver);*/
	printf("\n");
	struct assign node_array[ver];
	printf("Attaching IP addresses to all nodes for simulating transfer... \n \n");
	IP_generator(node_array,ver);
	printf("\n");
 struct packet p;
 int source_node=rand()%ver;
 printf("Selecting a source node to simulate the transfer... \n \n");
 printf("Source node is node-%d \n \n",source_node);
 int destination_node=rand()%ver;
 printf("Selecting a destination node for simulating the transfer... \n \n");
 printf("Destination node is node-%d \n \n",destination_node);
	int i;
	for(i=0;i<4;i++)
 {
 	p.source[i]=node_array[source_node].IP[i];
 	p.destination[i]=node_array[destination_node].IP[i];
	}
 
 printf("Source ip address %d.%d.%d.%d \n",p.source[0],p.source[1],p.source[2],p.source[3]);
 printf("Destination ip address %d.%d.%d.%d \n \n",p.destination[0],p.destination[1],p.destination[2],p.destination[3]);
 
 int *r;
	int min_dist[ver];
	printf("Applying Dijksta algorithm to find the minimum traffic from source to all nodes... \n \n");
	r=dijakstra(q,min_dist,source_node,ver);
	printf("\n");
for(int z=0;z<4;z++){
pro.source_IP[z]=p.source[z];
pro.destination_IP[z]=p.destination[z];
}
if(choice==0)
{	
struct network_parameters n;
	n.bandwidth=10;//kbps
printf("FOR COMPRESSED FILE\n");    
printf("Bandwidth= %d kbps \n",n.bandwidth);	
	n.avg_collision=10;//percent
	printf("Collision percentage = %d% \n",n.avg_collision);
	n.packet_size=2;//kb
	printf("Packet size=%d kb \n",n.packet_size);
	//n.distance=min_dist[destination_node];
//printf("Compressed File \n");
printf("\n");	
//function(n,decompressedsize);
//printf("uncompressed file\n");
printf("\n");
function(n,compressedsize);
}
else if(choice==1){	
struct network_parameters un;
	un.bandwidth=5;//bps
printf("FOR COMPRESSED FILE\n");    
printf("Bandwidth= %d Bps \n",un.bandwidth);	
	un.avg_collision=10;//percent
	printf("Collision percentage = %d% \n",un.avg_collision);
	un.packet_size=10;//bytes
	printf("Packet size= %d bytes \n",un.packet_size);
//printf("%f\n",aa);
int bb=(int)aa;	
//printf("%d\n",bb);
function(un,bb);
	}
return pro;
}

void send_file(FILE *fp, int clientSocket){
  int n;
  char data[SIZE] = {0};

  while(fgets(data, SIZE, fp) != NULL) {
    if (send(clientSocket, data, sizeof(data), 0) == -1) {
      perror("[-]Error in sending file.");
      exit(1);
    }
    bzero(data, SIZE);
  }
}
int main(){
dumb y;
int choice=0;
printf("\n");
printf("press 0 for text file\n");
printf("press 1 for image file\n");
printf("\n");
printf("enter your choice: ");
printf("\n");
//printf("FOR COMPRESSED FILE\n");

scanf("%d",&choice);
if(choice==0){
//int originalsize = 0, decompressedsize = 0;
    compressedsize = dohuffman('c');
    //decompressedsize = dohuffman('d');
    //printf("orignial filesize in bytes: %d\nCompressed filesize in bytes: %d\n", decompressedsize, compressedsize);
	}
else if(choice==1){

code(&aa);
}
else{
printf("Invalid Input\n");
exit(0);
}
y=simulation(choice,y);
//printf("FOR COMPRESSED FILE\n");

int clientSocket, ret;
	struct sockaddr_in serverAddr;
	char buffer[1024];
FILE *fp;
char *filename;
if(choice==0){  
filename = "com.txt";
}
else if(choice==1){
filename = "encoded_image.txt";
}
else{
exit(0);
}
	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(clientSocket < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Client Socket is created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
printf("\n");	
printf("[+]Connected to Server.\n");
printf("[+]Connection established between %d.%d.%d.%d and %d.%d.%d.%d\n",y.source_IP[0],y.source_IP[1],y.source_IP[2],y.source_IP[3],y.destination_IP[0],y.destination_IP[1],y.destination_IP[2],y.destination_IP[3]);


	while(1){
		printf("\t");
		// scanf("%s", &buffer[0]);
		// send(clientSocket, buffer, strlen(buffer), 0);
        
        fp = fopen(filename, "r");
  if (fp == NULL) {
    perror("[-]Error in reading file.");
    exit(1);
  }

  send_file(fp, clientSocket);
printf("\n");  
printf("[+]File data sent successfully.\n");

 	printf("[+]Closing the connection.\n");
   close(clientSocket);
		break;
        if(strcmp(buffer, ":exit") == 0){
			close(clientSocket);
			printf("[-]Disconnected from server.\n");
			exit(1);
		}

		// if(recv(clientSocket, buffer, 1024, 0) < 0){
		// 	printf("[-]Error in receiving data.\n");
		// }else{
		// 	printf("Server: \t%s\n", buffer);
		// }
	}

	return 0;
}
