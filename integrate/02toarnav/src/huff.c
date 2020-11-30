#include "huff.h"

struct node
{
	unsigned int weight; 
	unsigned short parent; 
	unsigned short child0, child1; // 0 / 1
	unsigned char codelength; 
};

void huff_cod(FILE * in, FILE * out)
{
	int i, j;
	
	/// Build frequency table
	unsigned int freqtable[256] = {0}; 
	unsigned char dirtab[256]; // Redirection table
	
	while(!feof(in)) 
		freqtable[fgetc(in)]++; 
	
	struct node tree[512000] = {0}; // store the tree
	unsigned short freenodes[256]; // List of free sub-trees
	unsigned short smallest, secondsmallest; 
	
	j = 0;
	for(i = 0; i < 256; i++) // Initialize leaves and free nodes
	{
		if(freqtable[i])
		{
			dirtab[i] = j;
			//printf("-%d: %d- ", i , dirtab[i]);
			freenodes[j] = j;
			tree[j].weight = freqtable[i];
			j++;
		}
	}
	unsigned short NxtFreeNode = j; // Next free node
	unsigned short leafnode = j - 1; // Last occupied position in freenodes

	while(leafnode > 0) // Iterate until the tree is complete
	{
		if(tree[freenodes[0]].weight < tree[freenodes[1]].weight) // Initialize smallest and b
		{
			smallest = 0;
			secondsmallest = 1;
		}
		else
		{
			secondsmallest = 0;
			smallest = 1;
		}
		
		for(i = 2; i <= leafnode; i++) // Find two smallest nodes
		{
			unsigned int w = tree[freenodes[i]].weight;
			if(w < tree[freenodes[smallest]].weight)
			{
				secondsmallest = smallest;
				smallest = i;
			}
			else
			{
				if(w < tree[freenodes[secondsmallest]].weight) secondsmallest = i;
			}
		}
		// Fill data
		unsigned short _a = freenodes[smallest];
		unsigned short _b = freenodes[secondsmallest];
		tree[_a].parent = NxtFreeNode;
		tree[_b].parent = NxtFreeNode;
		tree[NxtFreeNode].child0 = tree[_a].codelength <= tree[_b].codelength ? _a : _b;
		tree[NxtFreeNode].child1 = tree[_a].codelength > tree[_b].codelength ? _a : _b;
		tree[NxtFreeNode].weight = tree[_a].weight + tree[_b].weight;
		tree[NxtFreeNode].codelength = tree[_a].codelength + 1;
		
		freenodes[smallest] = NxtFreeNode++; // Replaces free node smallest with the newly created node
		freenodes[secondsmallest] = freenodes[leafnode--]; // Replaces free node b with the last free node;
		
	} // End tree building
	
#ifdef DEBUG
	for(i = 0; i < inode; i++) printf("%d> p = %hu, c0 = %hu, c1 = %hu, w = %6u, cl = %uc\n", i, tree[i].p, tree[i].c0, tree[i].c1, tree[i].w, tree[i].cl);
#endif
	
	/// Encode stream
	unsigned char stack[256]; // Traversed nodes stack
	unsigned char stackp = 0; // Stack pointer
	size_t fs = ftell(in); // File size
	
	fwrite(&fs, sizeof(size_t), 1, out); // Write uncompressed file size
	fwrite(freqtable, sizeof(unsigned int), 256, out); // Write frequency table
	rewind(in); // Back to the beginning
	
	unsigned char mask = 0x80; //128
	unsigned char byte = 0x00; //0
	while(1) // Encode all bytes
	{
		unsigned short inraw = fgetc(in);
		if(feof(in)) break;
		unsigned short curr = dirtab[inraw]; // 87 W
		
		while(tree[curr].parent) // Iterate backwards through the tree
		{
			stack[stackp++] = (curr == tree[tree[curr].parent].child1) ? 0xFF : 0x00; // Add bit to the pointer
			curr = tree[curr].parent; // 0xff is 255
		}
		
		while(stackp)
		{
			byte |= mask & stack[--stackp];
			mask >>= 1;
			if(!mask)
			{
				mask = 0x80;
				fputc(byte, out);
				byte = 0x00;
			}
		}
	}
	if(mask != 0x80) fputc(byte, out);
}

void huff_dec(FILE * in, FILE * out)
{
	int i, j;
	size_t length;
	size_t rcount;
	
	/// Build frequency table again
	unsigned int freqtable[256] = {0}; 
	unsigned char dirtab[256]; // Inverse redirection table
	
	if(fread(&length, sizeof(size_t), 1, in) < 1)
	{
		printf("Can't decode file.\n");
		exit(-1);
	}
	if(fread(freqtable, sizeof(unsigned int), 256, in) < 256) // Read frequency table
	{
		printf("Can't decode file.\n");
		exit(-1);
	}
	
	struct node tree[512000] = {0}; 
	unsigned short freenodes[256]; // List of free sub-trees
	unsigned short smallest, secondsmallest; // smallest = smallest node; b = second smallest
	
	j = 0;
	for(i = 0; i < 256; i++) // Initialize leaves and free nodes
	{
		if(freqtable[i])
		{
			dirtab[j] = i;
			freenodes[j] = j;
			tree[j].weight = freqtable[i];
			j++;
		}
	}
	unsigned short NxtFreeNode = j; // Next free node
	unsigned short leafnode = j - 1; // Last occupied position in freenodes

	while(leafnode > 0) // Iterate until the tree is complete
	{
		if(tree[freenodes[0]].weight < tree[freenodes[1]].weight) // Initialize smallest and b
		{
			smallest = 0;
			secondsmallest = 1;
		}
		else
		{
			secondsmallest = 0;
			smallest = 1;
		}
		
		for(i = 2; i <= leafnode; i++) // Find two smallest nodes
		{
			unsigned int w = tree[freenodes[i]].weight;
			if(w < tree[freenodes[smallest]].weight)
			{
				secondsmallest = smallest;
				smallest = i;
			}
			else
			{
				if(w < tree[freenodes[secondsmallest]].weight) secondsmallest = i;
			}
		}
		// Fill data
		unsigned short _a = freenodes[smallest];
		unsigned short _b = freenodes[secondsmallest];
		tree[_a].parent = NxtFreeNode;
		tree[_b].parent = NxtFreeNode;
		tree[NxtFreeNode].child0 = tree[_a].codelength <= tree[_b].codelength ? _a : _b;
		tree[NxtFreeNode].child1 = tree[_a].codelength > tree[_b].codelength ? _a : _b;
		tree[NxtFreeNode].weight = tree[_a].weight + tree[_b].weight;
		tree[NxtFreeNode].codelength = tree[_a].codelength + 1;
		
		freenodes[smallest] = NxtFreeNode++; // Replaces free node smallest with the newly created node
		freenodes[secondsmallest] = freenodes[leafnode--]; // Replaces free node b with the last free node;
		
	} // End tree building
	
#ifdef DEBUG
	for(i = 0; i < inode; i++) printf("%d> p = %hu, c0 = %hu, c1 = %hu, w = %6u, cl = %uc\n", i, tree[i].p, tree[i].c0, tree[i].c1, tree[i].w, tree[i].cl);
#endif
	
	/// Decode stream
		
	unsigned char mask = 0x80; // 128
	unsigned char byte = fgetc(in);
	for(i = 0; i < length; i++) // Decode to original size
	{
		if(feof(out))
		{
			printf("Can't decode file.\n");
			exit(-1);
		}
		unsigned short curr = NxtFreeNode - 1;
		while(tree[curr].codelength) // Follow the branches
		{
			curr = byte & mask ? tree[curr].child1 : tree[curr].child0;
			mask >>= 1;
			if(!mask)
			{
				mask = 0x80;
				byte = fgetc(in);
			}
		}
		fputc(dirtab[curr], out);
	}
}