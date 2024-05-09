/* main.c header file ----------------------------------------------------------------------

 Project: Exam PG3401 V24 - Oppgave 2
 Kandidatnummer : 38
 Description: Main source file for Oppgave 2

 The following code compiles by writing make creating executable, "oppgave_2".
 It will read text file named eksamen_v24_oppgave2.txt and produce a binary output file called output.bit.

 binToChar method at the end can be uncommented to verify the content of output.bin file.

 Executable can be run by writing: ./oppgave_2

--------------------------------------------------------------------------------------------*/

/*=== Standard C library: ===============================================================
=======================================================================================*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*=== Project specific include files: ===================================================
=======================================================================================*/

#include "./include/eksamen_v24_oppgave2_all_lower.h"
#include "./include/eksamen_v24_oppgave2_all_upper.h"
#include "./include/eksamen_v24_oppgave2_anagram.h"
#include "./include/eksamen_v24_oppgave2_disjoint.h"
#include "./include/eksamen_v24_oppgave2_heterogram.h"
#include "./include/eksamen_v24_oppgave2_palindrom.h"
#include "./include/oppgave2_constants.h"
#include "./include/oppgave2_bin_to_char.h"
#include "./include/oppgave2_task2_world_metadata.h"


int glob_config_variable = 0;

int main(void){

    char acInputBuffer[BUFFERSZE] = {0};
    char *pszWord = NULL;

    int iNumberOfBytes = 0, index = 1;

    long int currentPosition = 0;

    FILE *pFile = NULL;
    FILE *pFileWriter = NULL;

    struct TASK2_WORD_METADATA *sWordMetaData = NULL;

    pFile = fopen("./eksamen_v24_oppgave2.txt","r");
    pFileWriter = fopen("./output.bin", "wb");

    if(pFile != NULL && pFileWriter != NULL){

        do {

            //Moving pointer to the next word each round,
            //so I don't need a second File handler when I'm comparing word to words in file.
            //First round currentPosition = 0, so 0 in offset.
            fseek(pFile, currentPosition, SEEK_SET);

            memset(acInputBuffer,0,sizeof (acInputBuffer));
            pszWord = fgets(acInputBuffer, BUFFERSZE,pFile);

            iNumberOfBytes = 0;

            if(pszWord != NULL){

                //Storing the current position of File handler, so I can rewind later
                //without loosing track of where I am.
                currentPosition = ftell(pFile);

                //Looking for \r or \n to be replaced with \0.
                //Incrementing iNumberOfBytes each round,
                //so after the while loop I know how many bytes I have.
                while (pszWord[iNumberOfBytes] != '\0'){
                    if(pszWord[iNumberOfBytes] == '\r'|| pszWord[iNumberOfBytes] == '\n'){
                        pszWord[iNumberOfBytes] = '\0';
                        break;
                    }

                    iNumberOfBytes++;
                }

                //This should always match.
                if(iNumberOfBytes == strlen(pszWord)){

                    //Allocating struct + size of word + one byte for zero terminator
                    sWordMetaData = (struct TASK2_WORD_METADATA*)malloc(sizeof(struct TASK2_WORD_METADATA) + iNumberOfBytes + 1);

                    if(sWordMetaData != NULL){

                        memset(sWordMetaData,0,sizeof(struct TASK2_WORD_METADATA) + iNumberOfBytes + 1);

                        //The next lines until we rewind the file handler we're now
                        //setting all struct fields that are only determined by the word itself.
                        //We set IsAnagram and Disjoint to false, and then we set them to true if
                        //they get triggered later.

                        sWordMetaData->iSize = iNumberOfBytes;

                        strncpy(sWordMetaData->szWord,pszWord,iNumberOfBytes);
                        sWordMetaData->szWord[iNumberOfBytes] = '\0';

                        pszWord = NULL;
                        iNumberOfBytes = 0;

                        sWordMetaData->iIndex = index;
                        sWordMetaData->IsPalindrom = isPalindrome(sWordMetaData->szWord);
                        sWordMetaData->bIsHeterogram = isHeterogram(sWordMetaData->szWord);
                        sWordMetaData->bIsUppercase = isUpperCase(sWordMetaData->szWord);
                        sWordMetaData->bIsLowercase = isLowerCase(sWordMetaData->szWord);
                        sWordMetaData->bIsAnagram = false;
                        sWordMetaData->bIsDisjoint = false;


                        //Rewind the file handler, so it can be used to compare current word with
                        //the rest of the list.
                        rewind(pFile);

                        //This while loop wil loop through the file to check the word against the rest of the
                        //Words in the file. After this loop, all struct fields should be filled in.
                        memset(acInputBuffer,0,sizeof (acInputBuffer));
                        while (fgets(acInputBuffer,BUFFERSZE,pFile) != NULL){

                            if(strncmp(sWordMetaData->szWord,acInputBuffer, sWordMetaData->iSize) != 0){

                                int length = 0;
                                while (acInputBuffer[length] != '\0'){
                                    if(acInputBuffer[length] == '\r'|| acInputBuffer[length] == '\n'){
                                        acInputBuffer[length] = '\0';
                                        break;
                                    }

                                    length++;
                                }

                                if(isAnagram(sWordMetaData->szWord,acInputBuffer)){
                                    sWordMetaData->bIsAnagram = true;
                                }

                                if(areDisjoint(sWordMetaData->szWord,acInputBuffer)){
                                    sWordMetaData->bIsDisjoint = true;
                                }

                            }

                            memset(acInputBuffer,0,sizeof (acInputBuffer));
                        }

                        //At this point we are ready to write the current word to the binary file.
                        unsigned long iElementsWritten = fwrite(sWordMetaData,sizeof (struct TASK2_WORD_METADATA) + sWordMetaData->iSize + 1,1,pFileWriter);

                        if(iElementsWritten == 1){
                            //After successfully written 1 element to the file,
                            // we free memory and increment the index.
                            //We are now ready for next word.
                            index++;
                            free(sWordMetaData);
                            sWordMetaData = NULL;
                        } else{
                            printf("Failed when writing to file\n");
                            break;
                        }

                    } else{ //sWordMetaData != NULL
                        printf("Failed when allocating memory\n.");
                        break;
                    }

                } else{ //iNumberOfBytes == strlen(pszWord)
                    printf("Counted bytes didnt match with string length. Couldn't continue to malloc.\n");
                    break;
                }

            } else{ //pszWord != NULL
                printf("Reached end of file\n");
                break;
            }

        } while (1);


        if(sWordMetaData != NULL){
            free(sWordMetaData);
        }

        fclose(pFile);
        fclose(pFileWriter);

    }else{ //Error with file handler
        printf("Failed to open file.\n");
        return 1;
    }

    //Method to confirm that .bin file was written as expected.
    //Uncomment to check the newly created .bin file.


    int iRc = binToChar();

    if(iRc == 0){
        return 0;
    } else{
        return 1;
    }


    return 0;
}