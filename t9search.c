#include <stdio.h>
#include <stdlib.h>

#define MAX_CONTACTS 42
#define MAX_SYMB 100

//debug
void print_arr(char arr[], int length) {
    printf(">");
    for (int i = 0; i < length; i++) 
        printf("%c", arr[i]);
    printf("\n");
}


int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Bad input! Number of arguments:%d", argc);
        exit(-1);
    }

    //*2 because we contain NUMBER and NAME for each contact
    char contacts[MAX_CONTACTS*2][MAX_SYMB];
    char *sequence = argv[1];
    
    //Get contacts from stdin in contacts array
    char ch;
    int numOfContacts = 0; 
    int subindex = 0;
    while ((ch = getchar()) != EOF) {
        if (ch == '\n') {

            contacts[numOfContacts][subindex+1] = '\0'; //Define end of line

            numOfContacts++;
            subindex = 0;
            continue;
        } 
        contacts[numOfContacts][subindex] = ch;
        subindex++;
    }
}
