#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

//Method throughout the application to check if input are alpha or digit.
int IsValidInput(const char *szInput, int numberOrAlphaFlag) {

    int length = 0;

    if (szInput == NULL) {
        return 0;
    }

    length = strlen(szInput);
    if(length == 0){
        return 0;
    }

    switch (numberOrAlphaFlag) {
        case 1: // Check for alphabetic characters and spaces
            while (*szInput) {

                if (!isalpha((unsigned char)*szInput) && !isspace((unsigned char)*szInput)) {
                    return 0;
                }

                szInput++;
            }
            break;
        case 2: // Check for numeric characters
            while (*szInput) {
                if (!isdigit(*szInput)) {
                    return 0;
                }
                szInput++;
            }
            break;
        default:
            return 0;
    }

    return 1;
}

//Simple check to validate date
int IsValidDate(int day, int month, int year) {

    if (year < 2024) return 0;
    if (month < 1 || month > 12) return 0;
    if (day < 1 || day > 31) return 0;

    return 1;

}
//When searching for reservation and orders
//we are setting all characters to lower
void ToLower(char *pszString, char *pszStringLower){
    if(pszString == NULL){
        return;
    }

    while (*pszString){
        *pszStringLower = tolower(*pszString);
        pszString++;
        pszStringLower++;
    }

    *pszStringLower = '\0';
}

//Removes spaces, newline char and forces zero terminator to be the last char.
//Returns length of string minus zero terminator
static void SanitizeInput (char *psText){

    char *start;
    char *end;
    long length;

    start = psText;

    //Whitespace in front
    //Move until I hit first character which is not space.
    while (*start == ' '){
        ++start;
    }

    end = start;
    //Move end to newline character
    while (*end != '\0' && *end != '\n'){
        ++end;
    }

    if(*end == '\n'){
        *end = '\0';
    }

    //Moving end back from zero terminator
    end--;

    //Moving it back until it hits the last character
    while (end > start && *end == ' '){
        --end;
    }

    //Adding zero terminator
    *(end + 1) = '\0';

    //The pointer arithmetic doesn't include the last pointer when calculating addresses, so I have to add 1.
    length = end - start + 1;

    //Using memmove since im copying from same memory space. Safer than memcopy.
    //I add +1 to account for \0
    memmove(psText,start,length + 1);

}

void GetZeroTerminatedUserInput(char *pszBufferDestination, int iSize) {

    if (pszBufferDestination == NULL || iSize < 3) {
        printf("Size is too small or destination buffer is NULL.\n");
    } else {
        // Allocate memory for the input buffer
        char *szLocalBuffer = (char *)malloc(iSize * sizeof(char));
        if (szLocalBuffer == NULL) {
            printf("Failed to allocate memory\n");
        } else {
            memset(szLocalBuffer, 0, iSize);

            if (fgets(szLocalBuffer, iSize, stdin) == NULL) {
                printf("Failed to take input from user\n");
            } else {
                if (strchr(szLocalBuffer, '\n') == NULL) {
                    // Input too long for buffer, flush the rest
                    while (getchar() != '\n' && !feof(stdin)){

                    }
                }

                SanitizeInput(szLocalBuffer);

                // Copy the sanitized input to the destination buffer
                strncpy(pszBufferDestination, szLocalBuffer, iSize - 1);
                pszBufferDestination[iSize - 1] = '\0'; // Ensure null termination
            }
        }

        // Free allocated memory for szLocalBuffer
        if (szLocalBuffer != NULL) {
            free(szLocalBuffer);
        }
    }
}
