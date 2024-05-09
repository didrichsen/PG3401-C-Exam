#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "./include/oppgave2_task2_world_metadata.h"

//A function written to confirm that the .bin are written correct
//and can be mapped to TASK2_WORLD_METADATA struct.

int binToChar(void) {


    FILE *pFileReader = fopen("output.bin", "rb");
    if (pFileReader == NULL) {
        printf("Failed to open file for reading\n");
        return 1;
    }


    while (!feof(pFileReader)) {
        struct TASK2_WORD_METADATA readMetaData;


        if (fread(&readMetaData, sizeof(struct TASK2_WORD_METADATA), 1, pFileReader) != 1) {
            if (feof(pFileReader)) {
                // End of file reached, exit the loop
                break;
            } else {
                printf("Failed to read structure from file\n");
                fclose(pFileReader);
                return 1;
            }
        }

        char *readString = malloc(readMetaData.iSize + 1);
        if (readString == NULL) {
            printf("Memory allocation failed\n");
            fclose(pFileReader);
            return 1;
        }
        if (fread(readString, sizeof(char), readMetaData.iSize + 1, pFileReader) != readMetaData.iSize + 1) {
            printf("Failed to read string from file\n");
            free(readString);
            fclose(pFileReader);
            return 1;
        }
        readString[readMetaData.iSize] = '\0';

        printf("\n----%s----\n", readString);
        printf("Index: %d\n", readMetaData.iIndex);
        printf("size: %d\n", readMetaData.iSize);
        printf("Is palindrome: %d\n", readMetaData.IsPalindrom);
        printf("Is Heterogram: %d\n", readMetaData.bIsHeterogram);
        printf("Is Uppercase: %d\n", readMetaData.bIsUppercase);
        printf("Is Lowercase: %d\n", readMetaData.bIsLowercase);
        printf("Is anagram: %d\n", readMetaData.bIsAnagram);
        printf("Are Disjoint: %d\n", readMetaData.bIsDisjoint);
        printf("\n");

        free(readString);
    }


    fclose(pFileReader);

    return 0;
}