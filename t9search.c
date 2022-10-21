#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_CONTACTS 42
#define MAX_SYMB 100

char CharToNum(char);
int SaveContacts(char[MAX_CONTACTS*2][MAX_SYMB]);

// debug
void print_arr(char arr[], int length) {
    printf(">");
    for (int i = 0; i < length; i++) {
        printf("%c", arr[i]);
    }
    printf("\n");
}

// for (int i = 0; i < numOfContacts*2; i++) {
//     print_contact_cell(contacts[i]);
// }
void print_contact_cell(char arr[]) {
    printf(">");

    int i = 0;
    while (arr[i] != '\0') {
        printf("%c", arr[i]);
        i++;
    }

    printf("\n");
}
// debug

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Bad input! Number of arguments:%d", argc);
        exit(-1);
    }

    //Define the sequence for the search
    char *sequence = argv[1];
    int seqlen = strlen(argv[1]);

    char contactName[MAX_SYMB];
    char contactTel[MAX_SYMB];

    char ch;
    int cursymb;    
    while ((ch = getchar()) != EOF)
    {
        //compare method
    }
}

//Get num representation of char
//2 (abc), 3 (def), 4 (ghi), 5 (jkl), 6 (mno), 7 (pqrs), 8 (tuv), 9 (wxyz), 0 (+).
char CharToNum(char ch) {
    switch (tolower(ch))
    {
    case 'a': case 'b': case 'c':   
        return '2';
    case 'd': case 'e': case 'f':
        return '3';
    case 'g': case 'h': case 'i':
        return '4';
    case 'j': case 'k': case 'l':
        return '5';
    case 'm': case 'n': case 'o':
        return '6';
    case 'p': case 'q': case 'r': case 's':
        return '7';
    case 't': case 'u': case 'v':
        return '8';
    case 'w': case 'x': case 'y': case 'z':
        return '9';
    case '+':
        return '0';
    default:
        printf("Bad char!\n");
        return '\0';
    }
}