#include "stdio.h"

int sum(int a, int b) {
	return a + b;
}

float div(int a, int b){
	if (b == 0) {
		printf("Warning, b is 0\n");
		return 0;
	}
	return (float)a/b;
}

int main (int argc, char** argv)
{
	int a,b;
	char c;
	scanf(" %c",&c);
	scanf("%d",&a);
	scanf("%d",&b);

	printf("Stored values are: c:%c, a:%d, b:%d\n",c,a,b);
        if(c == '+'){
		printf("The sum of those numbers is: %d\n", sum(a, b));
	} else if (c == '/'){
		printf("The division of those numbers is: %g\n", div(a, b));
	} else {
		printf("Error, wrong operation\n");
	}
	return 0;
}
