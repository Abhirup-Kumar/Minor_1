// C Code for
// Image Compression
#include <stdio.h>
#include <stdlib.h>

// function to calculate word length
int codelen(char* code)
{
	int l = 0;
	while (*(code + l) != '\0')
		l++;
	return l;
}

// function to concatenate the words
void strconcat(char* str, char* parentcode, char add)
{
	int i = 0;
	while (*(parentcode + i) != '\0')
	{
		*(str + i) = *(parentcode + i);
		i++;
	}
	if (add != '2')
	{
		str[i] = add;
		str[i + 1] = '\0';
	}
	else
		str[i] = '\0';
}

// function to find fibonacci number
int fib(int n)
{
	if (n <= 1)
		return n;
	return fib(n - 1) + fib(n - 2);
}


