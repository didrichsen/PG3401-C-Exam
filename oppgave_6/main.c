/* main.c header file ----------------------------------------------------------------------

 Project: Exam PG3401 V24 - Oppgave 6
 Kandidatnummer : 38
 Description: Main source file for Oppgave 6

 The program takes in a .c file and turns simple while loops into for-loop,
 char* /char * turns into hungarian notations if they aren't all ready and
 lastly 3 spaces are switched out with /t.

 1) .c file is read into buffer
 2) We read buffer and writes for-loop and hungarian notations to a draf.c file
 3) We open the draft.c file and look for 3 space which is written to a file
 filename_beautified.c.

 Executable can be run with: ./oppgave_6 oppgave6_test.c

--------------------------------------------------------------------------------------------*/

/*=== Standard C library: ===============================================================
=======================================================================================*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

/*=== Project specific include files: ===================================================
=======================================================================================*/

#include "./include/oppgave6_generate_for_loop.h"
#include "./include/oppgave6_extract_body_from_while_loop.h"
#include "./include/oppgave6_extract_condition_from_while_loop.h"
#include "./include/oppgave6_extract_incrementer_from_while_loop.h"
#include "./include/oppgave6_extract_init_from_while_loop.h"
#include "./include/oppgave6_cleanup.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <input_filename>\n", argv[0]);
        return 1;
    }

    char *pszFileName = NULL;
    char *pszUpdatedFilename = NULL;
    char *pszFileContent = NULL;
    char *pszInit = NULL;
    char *pszCondition = NULL;
    char *pszIncrementer = NULL;
    char *pszBody = NULL;
    char *pszVariableName = NULL;

    int iOffsetBytes = 0, lengthOfFileName = 0;
    char *pStartOfName = NULL;
    int iRc = 0;
    long lFileSize = 0;

    FILE *pFileReadDraft = NULL;
    FILE *pFile = NULL;
    FILE *pFileWrite = NULL;

    lengthOfFileName = strlen(argv[1]);

    //Over the next lines we are creating the new filename - filename.beautified.c

    if (lengthOfFileName <= 0) {
        printf("Couldn't take length of filename.\n");
        return 1;
    }

    pszFileName = (char *)malloc(lengthOfFileName + 1);

    if (pszFileName == NULL) {
        printf("Failed to allocate memory for filename\n");
        return 1;
    } else{
        memset(pszFileName, 0, lengthOfFileName + 1);
        strncpy(pszFileName, argv[1], lengthOfFileName);
        pszFileName[lengthOfFileName] = '\0';

        pszUpdatedFilename = malloc(lengthOfFileName + strlen("_beautified") + 1); // _beautified + zero terminator

        if (pszUpdatedFilename == NULL) {
            printf("Error when opening file %d\n", errno);
            free(pszFileName);
            return 1;
        } else{

            memset(pszUpdatedFilename, 0, lengthOfFileName + strlen("_beautified") + 1);
            char *pCPosition = strstr(pszFileName, ".c");

            if (pCPosition == NULL) {
                printf("failed to find .c ending\n");
                free(pszUpdatedFilename);
                free(pszFileName);
                return 1;
            } else{
                int index = pCPosition - pszFileName;

                // We copy over everything except the .c part.
                strncpy(pszUpdatedFilename, pszFileName, index);
                pszUpdatedFilename[index] = '\0';
                strcat(pszUpdatedFilename, "_beautified.c");
                pszUpdatedFilename[lengthOfFileName + strlen("_beautified")] = '\0';
            }
        }
    }

    //Over the next lines we are getting the length of file content
    //We want to read it into a buffer (pszFileContent)

    pFile = fopen(pszFileName, "r");

    if (pFile == NULL) {
        printf("Failed to open file, %i\n", errno);
        free(pszFileName);
        free(pszUpdatedFilename);
        return 1;
    } else{
        iRc = fseek(pFile, 0, SEEK_END);

        if(iRc != 0){
            printf("Failed to open file\n");
            free(pszFileName);
            free(pszUpdatedFilename);
            fclose(pFile);
            return 1;
        } else{
            // We check size
            lFileSize = ftell(pFile);
            iRc = fseek(pFile, 0, SEEK_SET);

            if (iRc != 0) {
                printf("Failed to set pointer to start.\n");
                free(pszFileName);
                free(pszUpdatedFilename);
                fclose(pFile);
                return 1;
            } else{
                pszFileContent = (char *)malloc(lFileSize + 1);

                if (pszFileContent == NULL) {
                    printf("Failed to allocate memory for fileContent.\n");
                    free(pszFileName);
                    free(pszUpdatedFilename);
                    fclose(pFile);
                    return 1;
                } else{
                    memset(pszFileContent,0,lFileSize + 1);
                    size_t bytesRead = fread(pszFileContent, 1, lFileSize, pFile);

                    if (bytesRead != lFileSize) {
                        free(pszFileContent);
                        free(pszFileName);
                        free(pszUpdatedFilename);
                        fclose(pFile);
                        printf("Bytes read didn't match expected length\n");
                        return 1;
                    } else{
                        pszFileContent[lFileSize] = '\0';
                    }
                }
            }
        }
    }

    //Now we got the whole file in a buffer.
    //We now open a fileWriter so we can iterate over the buffer
    //and each time we find a while or a char *, we write a for loop and
    //update the variable name to be hungarian notation.

    //We will first be writing to a "draft" before we create the final
    //_beautified.c file when we are substituting spaces for tab

    pFileWrite = fopen("draft.c", "w");

    if (pFileWrite == NULL) {
        printf("Error on opening file: %i\n", errno);
        free(pszFileContent);
        free(pszFileName);
        free(pszUpdatedFilename);
        fclose(pFile);
        return 1;
    } else{
        for (int i = 0; i < lFileSize; i++) {
            if (pszFileContent[i] == 'w') {
                if (strncmp(&pszFileContent[i + 1], "hile", 4) == 0 &&
                    (pszFileContent[i + 5] == ' ')) {
                    // If we arrive here, there is a while-loop we need to switch out.
                    // We make sure to have a pointer to the start of the while loop.
                    // It's the baseline when extracting parts we need to store.
                    char *pWhile = &pszFileContent[i];
                    // Finding out where the 'i' should start up after we have inserted the for loop
                    int iFirstBracket = 0;
                    do {
                        if (pszFileContent[i] == '{') {
                            iFirstBracket++;
                        }
                        i++;
                    } while (iFirstBracket == 0);
                    // Now the 'i' stands at the first '{'
                    int iSecondBracket = 0;
                    do {
                        if (pszFileContent[i] == '/' && pszFileContent[i + 1] == '/') {
                            while (pszFileContent[i] != '\n') {
                                i++; // Advance 'i' to the next character until the newline is reached
                            }
                            i++; // Move 'i' past the newline character
                        }
                        if (pszFileContent[i] == '/' && pszFileContent[i + 1] == '*') {
                            // Skip until the end of the multi-line comment ()
                            while (!(pszFileContent[i] == '*' && pszFileContent[i + 1] == '/')) {
                                i++; // Advance 'i' to the next character until
                            }
                            i += 2; // Move 'i' past the end of the comment ()
                        }
                        if (pszFileContent[i] == '{') {
                            iSecondBracket--;
                        }
                        if (pszFileContent[i] == '}') {
                            iSecondBracket++;
                        }
                        i++;
                    } while (iSecondBracket != 1);
                    // We have incremented the 'i' to where we should start writing
                    // from after implementing the while loop.

                    // Within pszInit we find the init part. For example a = 2;
                    // We also find the iOffsetBytes which is the distance from a = 2 - > while(..)
                    // Using the offsetBytes later to place the FileWriter back to where it should start
                    // writing the for loop.
                    pszInit = ExtractInitFromWhile(pWhile, &iOffsetBytes);
                    if(pszInit == NULL){
                        continue;
                    }
                    // We go through the while parenthesis to extract the condition, a<b
                    pszCondition = ExtractConditionFromWhile(pWhile);
                    if(pszCondition == NULL){
                        Cleanup(&pszInit,NULL,NULL,NULL);
                        continue;
                    }
                    // We go through the body to find the incrementer
                    // We are passing in pszInit since we want to isolate the variable name: a
                    pszIncrementer = ExtractIncrementerFromWhile(pWhile, pszInit);
                    if(pszIncrementer == NULL){
                        Cleanup(&pszInit,&pszCondition,NULL,NULL);
                        continue;
                    }
                    // We are taking the size of the pszIncrement (variable name)
                    // Using it within ExtractBodyFromWhile to remove the incrementer
                    // from the body.
                    int iSizeOfIncrementer = strlen(pszIncrementer);
                    pszBody = ExtractBodyFromWhile(pWhile, iSizeOfIncrementer);

                    if(pszBody == NULL){
                        Cleanup(&pszInit,&pszCondition,&pszIncrementer, NULL);
                        continue;
                    }
                    // Generate the equivalent for loop
                    char *forLoop = GenerateForLoop(pszInit, pszCondition, pszIncrementer, pszBody);

                    if (forLoop == NULL) {
                        printf("Failed to generate for loop.\n");
                        Cleanup(&pszInit,&pszCondition,&pszIncrementer,&pszBody);
                        break;
                    } else {
                        fseek(pFileWrite, -iOffsetBytes, SEEK_CUR);
                        // Write the content of forLoop to the file
                        fprintf(pFileWrite, "%s", forLoop);
                        // Free the allocated memory for forLoop
                        free(forLoop);
                        // Continue to next iteration
                        Cleanup(&pszInit,&pszCondition,&pszIncrementer,&pszBody);
                        continue;
                    }
                }
            }

            if (pszFileContent[i] == 'c') {
                if (strncmp(&pszFileContent[i + 1], "har", 3) == 0 &&
                (pszFileContent[i + 4] == '*' || pszFileContent[i + 5] == '*')) {
                    // char *name;
                    // char* name[256];
                    // Anyway, we are going to write 6 bytes to file
                    // So we start with that, writing char* or char *
                    for (int j = 0; j < 6; j++) {
                        fputc(pszFileContent[i], pFileWrite);
                        i++;
                    }
                    // We are now pointing to where the psz should be written.
                    pStartOfName = &pszFileContent[i];
                    // We capitalize the first letter of the variable name.
                    if (*pStartOfName >= 'a' && *pStartOfName <= 'z') {
                        *pStartOfName = *pStartOfName - 32;
                    }
                    // Create a pointer to find the end of the variable name.
                    char *pEndOfName = pStartOfName;
                    // Looking for either ';' or ')'. ')' because of the main.
                    while (*pEndOfName != ';' && *pEndOfName != ')') {
                        pEndOfName++;
                    }
                    // Move pointer one step back.
                    // pEndOfName--;
                    // Making sure we adjust for brackets.
                    if (*pEndOfName == ']') {
                        while (*pEndOfName != '[') {
                            // If we find a bracket, we skip until we find a matching bracket.
                            pEndOfName--;
                        }
                        pEndOfName--;
                    }
                    // Now we can get the size of the variable (before psz)
                    int sizeOfVariable = 0;
                    sizeOfVariable = pEndOfName - pStartOfName;
                    char *psPrefix = "psz";
                    // Allocate memory for pszVariableName
                    pszVariableName = calloc(sizeOfVariable + strlen(psPrefix) + 1,sizeof (char )); // + psz + zero terminator
                    if (pszVariableName == NULL) {
                        printf("Failed to allocate memory for variable.\n");
                    } else {
                        // Copy and concatenate to create the variable.
                        // And write it to file.
                        //memset(pszVariableName, 0, strlen(pszVariableName ) + strlen(psPrefix));
                        strncpy(pszVariableName, psPrefix, 3);
                        strncat(pszVariableName, pStartOfName, sizeOfVariable);
                        fprintf(pFileWrite,"%s",pszVariableName);
                    }
                    // Free the pszVariable name
                    free(pszVariableName);
                    // Incrementing 'i' so it continues to write from after the variable.
                    // We print the ';' or the ')'
                    i += sizeOfVariable;
                    fprintf(pFileWrite, "%c", pszFileContent[i]);
                } else{
                    //Printing c if its not char*
                    fputc(pszFileContent[i], pFileWrite);
                }
            } else { // fileContent[i] == 'c'
                fputc(pszFileContent[i], pFileWrite);
            }
        } // for (int i = 0; i < fileSize; i++)
    }

    //At this point we have written updated variable names and for loops to the file
    //We close the file handler, open draft for read and we are now going to write to the
    //_beautified.c file

    //Now checking for three spaces that should be substitued for ine /t

    fclose(pFileWrite);

    pFileReadDraft = fopen("draft.c","r");
    pFileWrite = fopen(pszUpdatedFilename,"w");

    if(pFileReadDraft == NULL){
        printf("Failed to open file handler for read operation\n");
    } else{
        int c;
        int spaceCount = 0;
        while ((c = fgetc(pFileReadDraft)) != EOF) {
            if (c != ' ') {
                // If we previously counted spaces, write them out
                // before we write the current char after while
                while (spaceCount > 0) {
                    fputc(' ', pFileWrite);
                    spaceCount--;
                }
                fputc(c, pFileWrite);
            } else {
                spaceCount++; // Increment space count
                if (spaceCount == 3) {
                    fputc('\t', pFileWrite); // We are at last space, so write tab.
                    spaceCount = 0;
                }
            }
        }
    }



    //We are through and have written to _beautified.c
    //We can close file handlers and free allocated memory.

    fclose(pFileReadDraft);
    fclose(pFileWrite);
    fclose(pFile);

    free(pszFileContent);
    free(pszUpdatedFilename);
    free(pszFileName);

    //Deleting the draft.
    int status = remove("draft.c");

    if(status == 0){
        return 0;
    } else{
        printf("Failed to remove draft.c\n");
        return 1;
    }

}