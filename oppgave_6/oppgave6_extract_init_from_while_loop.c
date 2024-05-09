#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "include/oppgave6_extract_init_from_while_loop.h"

char* ExtractInitFromWhile(char* pStartOfWhile, int* iBytesOffset) {
    char *pszInit = NULL;

    // Find the end of the initialization part
    char *initEnd = pStartOfWhile;
    while (*initEnd != ';') {
        initEnd--;
    }
    initEnd--;

    // Find the start of the initialization part
    char *initStart = initEnd;
    while (*initStart != '\n') {
        initStart--;
    }
    initStart++;

    while (!isalpha(*initStart) && !isdigit(*initStart)) {
        initStart++;
    }

    *iBytesOffset = pStartOfWhile - initStart;

    // Calculate the size of the initialization part
    int iInitSize = initEnd - initStart + 1;

    // Allocate memory for pszInit
    pszInit = (char*) malloc(iInitSize + 1);
    if (pszInit == NULL) {
        printf("Failed to allocate memory.\n");
        return NULL;
    }

    // Copy the initialization part into pszInit
    strncpy(pszInit, initStart, iInitSize);
    pszInit[iInitSize] = '\0'; // Null-terminate the string

    return pszInit;
}
