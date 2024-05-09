#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "include/oppgave6_extract_body_from_while_loop.h"

char *ExtractBodyFromWhile(char *pStartOfWhile, int iSizeOfIncrementer){

    char *pBodyStart = pStartOfWhile;
    char *pszBody = NULL;
    while (*pBodyStart != '{'){
        pBodyStart++;
    }

    //We are now standing at the first {
    char* pBodyEnd = pBodyStart;

    int iCountBrackets = 0;

    do {

        if (*pBodyEnd == '/' && *(pBodyEnd + 1) == '/') {
            // Skip until past the newline character
            while (*pBodyEnd != '\n') {
                pBodyEnd++; // Move to the next character
            }
            pBodyEnd++; // Move past the newline character
        }

        if (*pBodyEnd == '/' && *(pBodyEnd + 1) == '*') {
            // Skip until past the */
            while (!(*pBodyEnd == '*' && *(pBodyEnd + 1) == '/')) {
                pBodyEnd++; // Move to the next character
            }
            pBodyEnd += 2; // Move past the */
        }

        if(*pBodyEnd == '{'){
            iCountBrackets++;
        }

        if(*pBodyEnd == '}'){
            iCountBrackets--;
        }

        pBodyEnd++;

    } while (iCountBrackets != 0);

    while (*pBodyEnd != ';'){
        pBodyEnd--;
    }

    pBodyEnd--;

    int iBodySize = pBodyEnd - pBodyStart + 1 - iSizeOfIncrementer;

    pszBody = malloc(iBodySize + 1);

    if(pszBody == NULL){
        printf("Failed to allocate memory.\n");
        return NULL;
    } else{
        strncpy(pszBody,pBodyStart,iBodySize);
        pszBody[iBodySize-1] = '}';
        pszBody[iBodySize] = '\0';
        return pszBody;
    }
}
