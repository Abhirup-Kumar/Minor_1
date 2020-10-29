// C program for Huffman Coding for text characters
#include <stdio.h> 
#include <stdlib.h> 

 
struct Node
{ 
	char data;  
	unsigned freq; 
	struct Node *left, *right; 
}; 

//collection of nodes...tree.. 
struct MinHeap
{  
	unsigned size; 
	unsigned capacity;  
	struct Node** array; 
}; 

 
// character and frequency for newnode.... 
struct Node* newNode(char data, unsigned freq) 
{ 
	struct Node* temp = (struct Node*)malloc(sizeof(struct Node)); 

	temp->left = temp->right = NULL; 
	temp->data = data; 
	temp->freq = freq; 

	return temp; 
}


void swapNode(struct Node** a, struct Node** b) 
{ 
	struct Node* t = *a; 
	*a = *b; 
	*b = t; 
} 


void minHeapify(struct MinHeap* minHeap, int idx) 
{
	int smallest = idx; 
	int left = 2 * idx + 1; 
	int right = 2 * idx + 2; 

	if (left < minHeap->size && minHeap->array[left]-> freq < minHeap->array[smallest]->freq) 
		smallest = left; 

	if (right < minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq) 
		smallest = right; 

	if (smallest != idx) 
	{ 
		swapNode(&minHeap->array[smallest], &minHeap->array[idx]); 
		minHeapify(minHeap, smallest); 
	} 
} 


int isSizeOne(struct MinHeap* minHeap) 
{ 
	return (minHeap->size == 1); 
} 


struct Node* extractMin(struct MinHeap* minHeap) 
{
	struct Node* temp = minHeap->array[0]; 
	minHeap->array[0] = minHeap->array[minHeap->size - 1]; 

	--minHeap->size; 
	minHeapify(minHeap, 0); 

	return temp; 
} 


// insert new node to Min Heap 
void insertNodeToHeap(struct MinHeap* minHeap, struct Node* minHeapNode) 
{ 
	++minHeap->size; 
	int i = minHeap->size - 1; 

	while (i && minHeapNode->freq < minHeap->array[(i - 1) / 2]->freq) 
	{
		minHeap->array[i] = minHeap->array[(i - 1) / 2]; 
		i = (i - 1) / 2; 
	} 

	minHeap->array[i] = minHeapNode; 
} 

void printArr(int arr[], int n) 
{ 
	int i; 
	for (i = 0; i < n; ++i) 
		printf("%d", arr[i]); 

	printf("\n"); 
} 


int isLeaf(struct Node* root) 
{
	return !(root->left) && !(root->right); 
} 


struct MinHeap* BuildMinHeap(char data[], int freq[], int size) 
{
	struct MinHeap* minHeap = (struct MinHeap*)malloc(sizeof(struct MinHeap)); 
	minHeap->size = 0; 
	minHeap->capacity = size; 
	minHeap->array = (struct Node**)malloc(minHeap->capacity * sizeof(struct Node*));

	for (int i = 0; i < size; ++i) 
		minHeap->array[i] = newNode(data[i], freq[i]); 

	minHeap->size = size; 
	int n = minHeap->size - 1; 
	int i=0; 
	for (i = (n - 1) / 2; i >= 0; --i) 
		minHeapify(minHeap, i); 

	return minHeap; 
} 


struct Node* buildHuffmanTree(char data[], int freq[], int size) 

{ 
	struct Node *left, *right, *top; 
	struct MinHeap* minHeap = BuildMinHeap(data, freq, size); 

	while (!isSizeOne(minHeap)) //if size is 1...we at top..
	{ 
		left = extractMin(minHeap); 
		right = extractMin(minHeap); 
		top = newNode('#', left->freq + right->freq); // a new parent node...

		top->left = left;  //both least value nodes, its children...
		top->right = right; 

		insertNodeToHeap(minHeap, top); 
	} 

	return extractMin(minHeap); //remaning node is root since size is one now and this returns least value node..root...
} 


void PrintHuffCodes(struct Node* root, int arr[], int top) 
{
	if (root->left) { 

		arr[top] = 0; 
		PrintHuffCodes(root->left, arr, top + 1); 
	} 

	if (root->right) { 

		arr[top] = 1; 
		PrintHuffCodes(root->right, arr, top + 1); 
	} 

	//leaf contains input char from file 
	if (isLeaf(root)) 
	{
		printf("%c: ", root->data); 
		printArr(arr, top); 
	} 
} 

void HuffmanCodes(char data[], int freq[], int size) 
{
	struct Node* root = buildHuffmanTree(data, freq, size); 
	int arr[1000], top = 0; 
	PrintHuffCodes(root, arr, top); 
} 


int main() 
{
	char arr[] = { 'M', 'I', 'N', 'O', 'R', 'T' , 'E', 'S', 'T'}; 
	int freq[] = { 7, 4, 15, 33, 18, 45, 19, 15, 13}; 

	int size = sizeof(arr) / sizeof(arr[0]); 
	HuffmanCodes(arr, freq, size); 
	
	return 0; 
} 
