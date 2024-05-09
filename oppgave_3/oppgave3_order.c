#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "./include/oppgave3_order.h"
#include "./include/oppgave3_input.h"


ORDER *CreateOrder(char *pszGuestName, int iOrderPrice, const char *pszOrder) {

    if (pszGuestName == NULL || pszOrder == NULL || iOrderPrice == 0) {

        return NULL; // Invalid input, cannot create order

    } else{

        size_t orderLength = strlen(pszOrder);

        if(orderLength <= 0){
            return NULL; //Cant create an order with no order.

        } else{

            char pszGuestNameLower[MAX_NAME_LENGTH];

            ORDER *pThis = (ORDER *)malloc(sizeof(ORDER) + orderLength + 1);

            if (pThis == NULL) {
                return NULL; // Memory allocation failed, cannot create order

            } else{
                // Copy guest name
                ToLower(pszGuestName,pszGuestNameLower);
                size_t guestNameLength = strlen(pszGuestNameLower);
                strncpy(pThis->acGuestName, pszGuestNameLower, guestNameLength);
                pThis->acGuestName[guestNameLength] = '\0';

                // Set order price
                pThis->iOrderPrice = iOrderPrice;

                // Copy order
                strncpy(pThis->acOrder, pszOrder, orderLength);
                pThis->acOrder[orderLength] = '\0';

                pThis->next = NULL;

                return pThis;
            }
        }
    }
}

// Function to add an order to a singly ordered linked list.
int AddOrder(ORDER **pList,char *pszGuestName, int iOrderPrice, const char *pszOrder) {
    if (pszGuestName == NULL || pszOrder == NULL || iOrderPrice == 0) {

        return ERROR; // Invalid input, return error

    } else{
        // Create order node
        ORDER *pThis = CreateOrder(pszGuestName, iOrderPrice, pszOrder);

        if (pThis == NULL) {

            return ERROR; // Failed to create order, return error

        } else{
            //Insert order into singly linked list.
            if(*pList == NULL){
                *pList = pThis;
            } else{
                ORDER *pCurrent = *pList;
                while (pCurrent->next != NULL){
                    pCurrent = pCurrent->next;
                }
                pCurrent->next = pThis;
                pThis->next = NULL;
            }

            return SUCCESS; // Order added successfully

        }
    }
}
// Function to view all orders.
void ViewAllOrders(ORDER *pOrder) {
    if (pOrder == NULL) {

        printf("No orders to show\r\n");
        return;

    } else{

        ORDER *pCurrent = pOrder;
        int iCounter = 1;

        while (pCurrent != NULL) {
            printf("Order, %d:\n", iCounter);
            printf("Guest Name: %s\n", pCurrent->acGuestName);
            printf("Order Price: %d\n", pCurrent->iOrderPrice);
            printf("Order Text: %s\n", pCurrent->acOrder);
            pCurrent = pCurrent->next;
            iCounter++;
            printf("\n");
        }
    }
}
// Deletes all orders in a singly order linked list.
void DeleteOrders(ORDER *pList) {
    if (pList == NULL) {

        return; // No orders to delete

    } else{

        ORDER *pCurrent = pList;
        ORDER *pTemp;

        while (pCurrent != NULL) {
            pTemp = pCurrent;
            pCurrent = pCurrent->next;
            free(pTemp);
        }
        pList = NULL;
    }
}
// A static method to check if a guests orders are printed
// When printing orders for a table, but wants separate recites.
static int IsGuestPrinted(const char *guestName, char **printedGuests, int count) {
    for (int i = 0; i < count; ++i) {
        if (strcmp(guestName, printedGuests[i]) == 0) {
            return 1;
        }
    }
    return 0;
}
//Prints all orders sorted per guest
void PrintOrdersPerGuest(ORDER *pOrder){

    if(pOrder != NULL){
        int maxGuestsPerTable = 50;
        char **printedGuests = malloc(maxGuestsPerTable * sizeof (char*));

        if(printedGuests == NULL){
            printf("Failed to allocate memory\n");
        } else{

            int count = 0;

            ORDER *pCurrent = pOrder;

            while (pCurrent != NULL){

                //If count is 0, we have not printed anyone. Doesn't need to check.
                //Also need count incremented before using it within IsGuetPrinted.
                if (count == 0 || IsGuestPrinted(pCurrent->acGuestName, printedGuests, count) == 0){

                    printedGuests[count] = malloc(MAX_NAME_LENGTH + 1);

                    if (printedGuests[count] == NULL) {
                        printf("Memory allocation failed\n");
                        break;
                    }else{
                        strncpy(printedGuests[count],pCurrent->acGuestName,MAX_NAME_LENGTH);

                        count++;

                        printf("\n----Orders for %s----\n", pCurrent->acGuestName);
                        PrintGuestOrders(pOrder,pCurrent->acGuestName);
                    }
                }

                pCurrent = pCurrent->next;
            }

            for (int i = 0; i < count; ++i) {
                free(printedGuests[i]);
            }

            pCurrent = NULL;
            free(printedGuests);
        }
    } else{
        printf("Order list is empty\n");
    }
}

//Helper method to print total price for an order.
int CalculateTotalCost(ORDER *pOrder) {
    int total = 0;
    while (pOrder != NULL) {
        total += pOrder->iOrderPrice;
        pOrder = pOrder->next;
    }
    return total;
}

//Prints one guets orders
void PrintGuestOrders(ORDER *pOrder, char *pszGuestName) {

    if (pOrder == NULL) {
        printf("No orders found for this reservation\n");
        return;
    } else{
        char pszGuestNameLower[MAX_NAME_LENGTH];
        ToLower(pszGuestName, pszGuestNameLower);

        int iTotalCost = 0;
        ORDER *pCurrent = pOrder;

        while (pCurrent != NULL) {

            if (strcmp(pCurrent->acGuestName, pszGuestNameLower) == 0) {
                printf("%s : %d ,-\n", pCurrent->acOrder, pCurrent->iOrderPrice);
                iTotalCost += pCurrent->iOrderPrice;
            }

            pCurrent = pCurrent->next;
        }

        if (iTotalCost != 0) {
            printf("Total cost for %s: %d,-\n", pszGuestName, iTotalCost);
        } else {
            printf("No orders found for guest %s\n", pszGuestName);
        }
    }
}