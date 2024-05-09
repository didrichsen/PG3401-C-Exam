#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "include/oppgave6_extract_incrementer_from_while_loop.h"

char* ExtractIncrementerFromWhile(char *pStartOfWhile, char *pszIncrementer){

    //This method is going to find the incrementer within the body (a++,a--,a+=2 etc)
    //Now we only got variableName = something. We need variable name and incrementer.
    //We will search for this in the body.
    char *pszVariableEnd = pszIncrementer;
    char *pszCondition = NULL;
    int iBytes = 0;

    //We check size of the incrementer which we have captured earlier.
    while (*pszVariableEnd != ' '){
        iBytes++;
        pszVariableEnd++;
    }

    char *pszExpression = malloc(iBytes + 1);

    if(pszExpression == NULL){
        printf("Failed to allocate memory\n");
        return NULL;
    } else {
        //At this point we got the variable name.
        strncpy(pszExpression, pszIncrementer, iBytes);
        pszExpression[iBytes] = '\0';

        char* pStartOfCondition = NULL;

        iBytes = strlen(pszExpression);

        char *pTemp = pStartOfWhile;

        int iFoundOperator = 0;

        //we are here looking for the actual incrementer in the body
        //We do a strstr on pTemp which points to the start of the while loop.
        //We can encounter incrementer wihthin the body, so we need to check if its followed by incrementers
        while ((pStartOfCondition = strstr(pTemp, pszExpression)) != NULL && iFoundOperator == 0) {
            char firstChar = *(pStartOfCondition + iBytes);
            char secondChar = *(pStartOfCondition + iBytes +1);

            switch (firstChar) {
                case '+':
                    if (secondChar == '+' || secondChar == '=') {
                        iFoundOperator = 1;
                        break;
                    }
                case '-':
                    if (secondChar == '-' || secondChar == '=') {
                        iFoundOperator = 1;
                        break;
                    }
                default:
                    pTemp += 1;
                    continue;
            }
        }

        free(pszExpression);
        pszExpression = NULL;

        char* pEndOfCondition = pStartOfCondition;

        while (*pEndOfCondition != ';' && *pEndOfCondition != '\0') {
            // Move to the next character
            pEndOfCondition++;
        }

        pEndOfCondition--;

        int iExpressionSize = pEndOfCondition - pStartOfCondition + 1;

        pszCondition = malloc(iExpressionSize + 1);

        if(pszCondition == NULL){
            printf("Failed to allocate\n");
            return NULL;

        } else{
            //Here we are returning the expression, so it can be used when we construct our while loop.
            strncpy(pszCondition,pStartOfCondition,iExpressionSize);
            pszCondition[iExpressionSize] = '\0';
            return pszCondition;
        }
    }
    return NULL;
}
