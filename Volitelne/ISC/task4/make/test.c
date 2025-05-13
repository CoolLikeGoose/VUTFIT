#include "functions.h"
#include "stdio.h"

int main()
{
	int array[10]={1,2,3,4,5,6,7,6,5,4};

	int fails=0;
	if (sum(array,10)!=43)
	{
		fails++;
	}

	if (min(array,10)!=1)
	{
		fails++;
	}

	if (max(array,10)!=7)
	{
		fails++;
	}


	printf ("Failed test count: %d\n",fails);
	return fails;

}