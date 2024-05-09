#include <stdlib.h>
#include <stdio.h>

#include "./include/oppgave6_cleanup.h"

//A cleanup function to handle memory when creating while loop.

void Cleanup(char **pszInit, char **pszCondition, char **pszIncrementer, char **pszBody) {
    if (pszInit != NULL) {
        free(*pszInit);
        *pszInit = NULL;
    }
    if (pszCondition != NULL) {
        free(*pszCondition);
        *pszCondition = NULL;
    }
    if (pszIncrementer != NULL) {
        free(*pszIncrementer);
        *pszIncrementer = NULL;
    }
    if (pszBody != NULL) {
        free(*pszBody);
        *pszBody = NULL;
    }
}