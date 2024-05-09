#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

char *GenerateForLoop(char *pszInit, char *pszCondition, char *pszIncrementer, char *pszBody) {

    char *forCharacters = "for(;;)\\n";

    int totalLength = strlen(pszInit) + strlen(pszCondition) + strlen(pszIncrementer) + strlen(pszBody) +
            strlen(forCharacters);

    char *forLoop = (char *) malloc(totalLength * sizeof(char) + 1);

    if (forLoop == NULL) {
        printf("Failed to allocate memory.\n");
        return NULL;
    }

    forLoop[totalLength] = '\0';

    sprintf(forLoop, "for(%s;%s;%s)%s\n", pszInit, pszCondition, pszIncrementer, pszBody);
    return forLoop;
}