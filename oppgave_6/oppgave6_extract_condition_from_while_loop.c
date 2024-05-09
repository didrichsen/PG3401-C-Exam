#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "include/oppgave6_extract_condition_from_while_loop.h"

char* ExtractConditionFromWhile(char* pStartOfWhile){

    char *pszExpression = NULL;

    char *pExpressionStart = pStartOfWhile;

    while (*pExpressionStart != '('){
        pExpressionStart++;
    }
    pExpressionStart++;

    char *pExpressionEnd = pExpressionStart;

    while (*pExpressionEnd != ')'){
        pExpressionEnd++;
    }

    pExpressionEnd--;

    int iExpressionSize = pExpressionEnd - pExpressionStart + 1;

    // Allocate memory for iExpressionSize
    pszExpression = (char*) malloc(iExpressionSize + 1);
    if (pszExpression == NULL) {
        printf("Failed to allocate memory.\n");
        return NULL;
    }

    // Copy the initialization part into pszInit
    strncpy(pszExpression, pExpressionStart, iExpressionSize);
    pszExpression[iExpressionSize] = '\0'; // Null-terminate the string

    return pszExpression;


}
