#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

//#define MAX_CONTACTS 42
#define MAX_SYMB 101 

char CharToNum(char);
int CompareContant(char[2][MAX_SYMB], char*, int);
void PrintEverything();

//The program can support an unlimited number of contacts 
//because the comparison and output is done directly while reading the data, 
//but the limit of 100 characters per line is still applicable
int main(int argc, char *argv[]) {
    if (argc > 2) {
        printf("ERROR: Bad input! Number of arguments: %d\n", argc);
        return 0;
    } else if (argc == 1) {
        PrintEverything();
        return 0;
    }

    //Define the sequence for the search
    char *searchSequence = argv[1];
    int seqlen = strlen(argv[1]);

    //0 - name array; 1 - number array
    char contact[2][MAX_SYMB];
    int curLen = 0;
    int dataBias = 0;

    char ch;
    int finded = 0; //We can print the number of finded contacts or print "Not finded" 
    while ((ch = getchar()) != EOF)
    {
        //Max symbols == 100 & 101 symbol is \0
        if (ch == '\n' || curLen == 101) {
            //clear stdin if string is bigger than 100 symbols
            if (curLen == 101) while ((ch = getchar()) != '\n');
            contact[dataBias][curLen] = '\0';

            //Compare with searchSequence 
            //if name & number of the contact is already saved
            if (dataBias == 1) {
                finded += CompareContant(contact, searchSequence, seqlen);
            }

            curLen = 0;
            dataBias = dataBias == 0 ? 1 : 0;
            continue;
        }
        contact[dataBias][curLen] = ch;
        curLen++;
    }
    //Check for the last contact
    contact[dataBias][curLen] = '\0'; 
    if (dataBias == 1) {
        finded += CompareContant(contact, searchSequence, seqlen);
    } else {
        printf("ERROR: Incorrect data\n");
        printf("Maybe one of the contacts does not contain the number or name\n");
    }

    //In case when no contact is equal to a sequence
    if (finded == 0) 
        printf("Not found\n");

    return 0;
}

//We can get rid of this function by adding an additional bool variable to main, 
//but this makes the code less readable, so I decided to keep it
//If argc == 1 -> Print everything from stdin
void PrintEverything() {
    char contact[2][MAX_SYMB];
    char ch;
    int bias = 0;
    int i = 0;
    while ((ch = getchar()) != EOF) {
        if (ch == '\n' || i == 101) {
            if (i == 101) while ((ch = getchar()) != '\n');
            contact[bias][i] = '\0';

            if (bias == 1) {
                printf("%s, %s\n", contact[0], contact[1]);
            }

            i = 0;
            bias = bias == 0 ? 1 : 0;
            continue;
        }
        contact[bias][i] = ch;
        i++;
    }
    contact[bias][i] = '\0'; 
    if (bias == 1) {
        printf("%s, %s\n", contact[0], contact[1]);
    } else {
        printf("ERROR: Incorrect data\n");
        printf("Maybe one of the contacts does not contain the number or name\n");
    }
}

int CheckForPlus(char ch) {
    if (ch == '+') return '0';
    return ch;
}

int CompareContant(char contact[2][MAX_SYMB], char *sequence, int seqLen) {
    //Searching for compliance with desired sequence in contact number
    int searchInd = 0;
    int i = 0;
    while ((contact[1][i]) != '\0') {
        if (CheckForPlus(contact[1][i]) == sequence[searchInd]) {
            searchInd++;
            if (searchInd == seqLen) {
                printf("%s, %s\n", contact[0], contact[1]);
                return 1;
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
                printf("%s, %s\n", contact[0], contact[1]);
                return 1;
            }
        } else if (searchInd != 0) {
            searchInd = 0;
        }
        i++;
    }

    return 0;
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
        return 'N'; //For characters not supported by the keyboard (e.g. dot)
    }
}