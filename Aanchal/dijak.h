#include<stdio.h>
#include<stdlib.h>
#include<limits.h>

struct packet{
int source[4];
int destination[4];
int length;
};
struct assign{
	int n;
	int IP[4];
};

struct network_parameters{
	int bandwidth;
	int packet_size;
	int distance;
	int avg_collision;
};

int vertex(int upper,int lower)
{
	int v=(rand()%(upper-lower+1))+lower;
	return v;
}
int ** edges(int upper, int lower,int size)
{
	int i,j;
	int** a=(int **)malloc(size * sizeof(int *));
	for(i=0;i<size;i++)
	{
	 a[i]=(int *)malloc(size * sizeof(int)); }
	 for(i=0;i<size;i++)
	 {
		for(j=0;j<size;j++)
		{
			int num=(rand()%(upper-lower+1))+lower;
			a[i][j]=num;
		}
	}
	for(i=0;i<size;i++)
	{
		for(j=0;j<size;j++)
		{
			a[j][i]=a[i][j];
		}
	}
	return a;
}
void disp(int ** a,int size)
{
	int i,j;
	for(i=0;i<size;++i)
	{
		for(j=0;j<size;++j)
		{
			printf("%4d ",a[i][j]); 
		}
		printf("\n");
	}
}

void IP_generator(struct assign * a, int size)
{
	int i,j;
	for(i=0;i<size;i++)
	{
		a[i].n=i;
			for(j=0;j<4;j++)
		{
	 int x=(rand()%255);
		a[i].IP[j]=x;
		}
	}
 for(i=0;i<size;i++)
	{
	printf("IP address of node %d is %d.",a[i].n,a[i].IP[0]);
	printf("%d.",a[i].IP[1]);
	printf("%d.",a[i].IP[2]);
	printf("%d",a[i].IP[3]);
	printf("\n");
}
}

int min_dist(int * a,int * visited, int v)
{
	int i;
	int min=INT_MAX;
	int index;
	for(i=0;i<v;i++)
	{
		if(visited[i]==0 && a[i]<=min)
		{
			min=a[i];
			index=i;
		}
	}return index;
}

void Path(int * parent, int dest) 
{ 
 if (parent[dest] == - 1) 
  {return;} 
 Path(parent, parent[dest]);  
 printf("%d ", dest); 
} 

int sol(int* dist, int ver, int* parent,int source_node) 
{ 
 int i;
 int source = source_node; 
 printf("Vertex    Traffic    Path"); 
 for (i = 0; i < ver; i++) 
 { 
  printf("\n%d -> %d  %4d      %4d ", source, i, dist[i], source); 
  Path(parent, i); 
 } 
} 

int * dijakstra(int ** a, int * dist,int source,int v)
{ 
 int i,j;
 int visited[v];
 int parent[v];
 for(i=0;i<v;i++)
 {
 	parent[i]= -1;
	 dist[i]=INT_MAX;
	 visited[i]=0;
 }
 dist[source]=0;
 for(i=0;i<v-1;i++)
 {
 	int x=min_dist(dist,visited,v);
 	visited[x]=1;
 	for(j=0;j<v;j++)
 	{
 		if(!visited[j] && a[x][j] && dist[x]!=INT_MAX && dist[x]+a[x][j]<dist[j])
 		{
 			parent[j]=x;
			dist[j]=dist[x]+a[x][j];
		}}
	}sol(dist,v,parent,source);
	printf("\n");
	return dist;
}



void function(struct network_parameters x,int size)
{
	int packets=size/x.packet_size;
	printf("The number of packets created is %d. \n",packets);
	int resend= 0.1*packets;
	printf("Packets that need to be resend %d. \n",resend);
	packets=resend+packets;
	float transfer_time=(packets*x.packet_size)/x.bandwidth;
	printf("Total transfer time = %f seconds. \n",transfer_time);
}