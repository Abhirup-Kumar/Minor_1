#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "dijak.h"
typedef struct Something{
	int source_IP[4];
	int destination_IP[4];
}dumb;
dumb simulation(dumb pro)
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
	printf("Matrix representation of the grapg generated \n \n");
	disp(q,ver);
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
	struct network_parameters n;
	n.bandwidth=10;//kbps
    printf("Bandwidth= %d kbps \n",n.bandwidth);	
	n.avg_collision=10;//percent
	printf("Collision percentage = %d% \n",n.avg_collision);
	n.packet_size=2;//kb
	printf("Packet size=%d kb \n",n.packet_size);
	//n.distance=min_dist[destination_node];
	function(n,100);
	struct network_parameters un;
	un.bandwidth=200;//bps
    printf("Bandwidth= %d bps \n",un.bandwidth);	
	un.avg_collision=10;//percent
	printf("Collision percentage = %d% \n",un.avg_collision);
	un.packet_size=50;//bytes
	printf("Packet size= %d bytes \n",un.packet_size);
	function(un,1024);
	for(i=0;i<4;i++)
	{
		pro.source_IP[i]=p.source[i];
		pro.destination_IP[i]=p.destination[i];
	}
	return pro;
}

int main()
{

	dumb hi;
	hi=simulation(hi);

    //printf("%d.%d.%d.%d",hi.source_IP[0],hi.source_IP[1],hi.source_IP[2],hi.source_IP[3]);
	
    return 0;
}