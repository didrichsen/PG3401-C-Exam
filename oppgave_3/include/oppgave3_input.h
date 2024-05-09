#ifndef PG3401_V24_38_EKSAMEN_V24_OPPGAVE3_INPUT_H
#define PG3401_V24_38_EKSAMEN_V24_OPPGAVE3_INPUT_H

#define MAX_NAME_LENGTH 256
#define BUFFER_SIZE 256
#define DISH_LENGTH 256

#define SUCCESS 0
#define ERROR (-1)

int IsValidInput(const char *szName, int numberOrAlphaFlag);
int IsValidDate(int day, int month, int year);
void ToLower(char *pszString, char *pszStringLower);
void GetZeroTerminatedUserInput(char *pszBufferDestination, int iSize);

#endif //PG3401_V24_38_EKSAMEN_V24_OPPGAVE3_INPUT_H
