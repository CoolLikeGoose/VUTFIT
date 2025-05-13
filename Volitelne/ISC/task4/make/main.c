#include "functions.h"
#include "stdio.h"

const int arrayLength=5;


int main()
{
	int array[arrayLength];
	for(int i=0;i<arrayLength;i++)
	{
		scanf("%d",&array[i]);
	}
	printf("Sum: %d, Min: %d, Max: %d",sum(array,arrayLength),min(array,arrayLength),max(array,arrayLength));
	return 0;
}