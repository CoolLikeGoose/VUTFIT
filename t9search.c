#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#define MAX_CONTACTS 42
#define MAX_SYMB 100

char CharToNum(char);
int SaveContacts(char[MAX_CONTACTS*2][MAX_SYMB]);
void CompareCurContant(char[2][MAX_SYMB], char*, int);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Bad input! Number of arguments:%d", argc);
        exit(-1);
    }

    //Define the sequence for the search
    char *searchSequence = argv[1];
    int seqlen = strlen(argv[1]);

    //1 - name; 2 - number
    char contact[2][MAX_SYMB];

    int curLen = 0;
    int dataBias = 0;

    char ch;
    while ((ch = getchar()) != EOF)
    {
        if (ch == '\n') { //pizdec novy radek
            contact[dataBias][curLen] = '\0';

            //Compare with searchSequence 
            //if number of contact is already saved
            if (dataBias == 1) {
                CompareCurContant(contact, searchSequence, seqlen);
            }

            curLen = 0;
            dataBias = dataBias == 0 ? 1 : 0;
            continue;
        }
        contact[dataBias][curLen] = ch; //tolower(ch);
        curLen++;
    }
}

void PrintContact(char contact[2][MAX_SYMB]) {
    int i = 0;
    while (contact[0][i] != '\0') {
        printf("%c", contact[0][i]);
        i++;
    }
    printf(", ");
    i = 0;
    while (contact[1][i] != '\0') {
        printf("%c", contact[1][i]);
        i++;
    }
    printf("\n");
}

void CompareCurContant(char contact[2][MAX_SYMB], char *sequence, int seqLen) {
    //Searching for compliance with desired sequence in contact number
    int searchInd = 0;
    int i = 0;
    while ((contact[1][i]) != '\0') {
        if (contact[1][i] == sequence[searchInd]) {
            searchInd++;
            if (searchInd == seqLen) {
                PrintContact(contact);
                return;
            }
        } else if (searchInd != 0) {
            searchInd = 0;
        }
        i++;
    }

    //Searching for compliance with desired sequence in contact name

    searchInd = 0;
    i = 0;
    while ((contact[0][i]) != '\0') {
        if (CharToNum(contact[0][i]) == sequence[searchInd]) {
            searchInd++;
            if (searchInd == seqLen) {
                PrintContact(contact);
                return;
            }
        } else if (searchInd != 0) {
            searchInd = 0;
        }
        i++;
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
    case ' ': case '.':
        return 'N';
    default:
        printf("Bad char! --->%c\n", ch);
        return '\0';
    }
}