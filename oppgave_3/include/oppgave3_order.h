#ifndef PG3401_V24_38_OPPGAVE3_ORDER_H
#define PG3401_V24_38_OPPGAVE3_ORDER_H

#include "oppgave3_input.h"

typedef struct _ORDER{

    struct _ORDER *next;
    char acGuestName[MAX_NAME_LENGTH];
    int iOrderPrice;
    char acOrder[];

}ORDER;

ORDER *CreateOrder(char *pszGuestName, int iOrderPrice, const char *pszOrder);
int AddOrder(ORDER **pList,char *pszGuestName, int iOrderPrice, const char *pszOrder);
void ViewAllOrders(ORDER *pOrder);
void DeleteOrders(ORDER *pList);
int CalculateTotalCost(ORDER *pOrder);
void PrintOrdersPerGuest(ORDER *pOrder);
void PrintGuestOrders(ORDER *pOrder, char *pszGuestName);

#endif //PG3401_V24_38_OPPGAVE3_ORDER_H
