#include "stdio.h"

int add (int a, int b)
{
	return a+b;
}
struct result{
	int err;
	int res;
	int rem;
	};

int div (int a, int b, int* res, int* rem)
{
	int rv;
	if (b==0)
	{
		rv=1;
	}
	else
	{
		*res=a/b;
		*rem=a%b;
		rv=0;
	}
	return rv;
}

int main (int argc, char** argv)
{
	int a,b;
	char c;
	scanf(" %c",&c);
	scanf("%d",&a);
	scanf("%d",&b);

	if (c=='+')
	{
		printf("%d\n",add(a,b));
	}
	else if (c=='/')
	{
		int *res,*rem;
		
		if (div(a,b,res,rem)==0)
		{
			printf("res: %d\nrem: %d\n",res,rem);
		}
		else
		{
			printf("Division by 0 is not defined\n");
		}

	}
	else
	{
		printf("Unknown operation, use + for addition or / for division\n");
	}

	return 0;
}

