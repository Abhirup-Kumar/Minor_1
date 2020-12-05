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
#define SIZE 1024
#define PORT 4444
int decompressedsize=0;
//int choice;
float dd=0.0;
void write_file(int sockfd){
  int n;
  FILE *fp;
  char *filename = "recv.txt";
  char buffer[SIZE];

  fp = fopen(filename, "w");
  while (1) {
    n = recv(sockfd, buffer, SIZE, 0);
    if (n <= 0){
      break;
      return;
    }
    fprintf(fp, "%s", buffer);
    bzero(buffer, SIZE);
  }
  return;
}
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

int main(){
decode(&dd);


int sockfd, ret;
	 struct sockaddr_in serverAddr;

	int newSocket;
	struct sockaddr_in newAddr;

	socklen_t addr_size;

	char buffer[1024];
	pid_t childpid;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Server Socket is created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("[-]Error in binding.\n");
		exit(1);
	}
	printf("[+]Bind to port %d\n", 4444);

	if(listen(sockfd, 10) == 0){
		printf("[+]Listening....\n");
	}else{
		printf("[-]Error in binding.\n");
	}


	while(1){
		newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
		write_file(newSocket);
  //printf("[+]Data written in the file successfully.\n");
int choice;
printf("Enter 1 if it is image\n");
printf("Enter 0 if it is text\n");

printf("Enter your choice:\n");

scanf("%d",&choice);

if(choice==1){ 
struct network_parameters para;
para.bandwidth=5;//bps
printf("\n");    
printf("Bandwidth= %d bps \n",para.bandwidth);	
	para.avg_collision=10;//percent
	printf("Collision percentage = %d% \n",para.avg_collision);
	para.packet_size=10;//kb
	printf("Packet size=%d kb \n",para.packet_size);	
printf("%f",dd);
int gg=(int)dd;	
function(para,gg);   
}     
else if(choice==0){
decompressedsize=dohuffman('d');
struct network_parameters para;
para.bandwidth=10;//kbps
printf("\n");    
printf("Bandwidth= %d kbps \n",para.bandwidth);	
	para.avg_collision=10;//percent
	printf("Collision percentage = %d% \n",para.avg_collision);
	para.packet_size=2;//kb
	printf("Packet size=%d kb \n",para.packet_size);	
function(para,decompressedsize);
}
else{
printf("Wrong Choice\n");
exit(0);
}   
printf("\n");
printf("[+]Data written in the file successfully.\n");  
if(newSocket < 0){
			exit(1);
		}
printf("\n");		
printf("Connection accepted from client on port no:%d\n",  ntohs(newAddr.sin_port));
printf("\n");

		if((childpid = fork()) == 0){
			close(sockfd);

			while(1){
				recv(newSocket, buffer, 1024, 0);
				if(strcmp(buffer, ":exit") == 0){
					printf("Disconnected from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
					break;
				}else{
					printf("file sent %s\n",buffer);
					send(newSocket, buffer, strlen(buffer), 0);
					bzero(buffer, sizeof(buffer));
break;				
}
			}
		}

	}

	close(newSocket);


	return 0;
}
