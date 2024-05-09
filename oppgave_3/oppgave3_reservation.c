#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "./include/oppgave3_reservation.h"
#include "./include/oppgave3_order.h"
#include "./include/oppgave3_input.h"

static RESERVATION *CreateReservation(int iTableNumber, int iNumberOfSeats,int iDay,int iMonth, int iYear, int iHour, int iMinutes, char *pszGuestName) {

    RESERVATION *pThis = NULL;
    char szGuestNameToLower[MAX_NAME_LENGTH];

    if (pszGuestName == NULL || iTableNumber == 0 || iNumberOfSeats == 0) {

        return NULL;

    } else if (IsValidDate(iDay,iMonth,iYear) == 0){

        return NULL;

    } else{

        size_t name_length = strlen(pszGuestName);
        pThis = (RESERVATION *)malloc(sizeof(RESERVATION) + name_length + 1);

        if (pThis == NULL) {

            return NULL;

        } else{

            memset(pThis, 0, sizeof(RESERVATION) + name_length + 1);

            pThis->iTableNumber = iTableNumber;
            pThis->iNumberOfSeats = iNumberOfSeats;
            pThis->iDay = iDay;
            pThis->iMonth = iMonth;
            pThis->iYear = iYear;
            pThis->iHour = iHour;
            pThis->iMinute = iMinutes;

            ToLower(pszGuestName,szGuestNameToLower);
            strncpy(pThis->acGuestName,szGuestNameToLower,name_length);
            pThis->acGuestName[name_length] = '\0';

            pThis->pNextReservation = NULL;
            pThis->pPrevReservation = NULL;
            pThis->pOrder = NULL;

            return pThis;

        }
    }
}

// Inserting a reservation at the head of the doubly linked list.
int InsertReservation(RESERVATION_LIST *pList, int iTableNumber, int iNumberOfSeats,int iDay,int iMonth,int iYear, int iHour, int iMinutes,char *acGuestName) {

    if (acGuestName == NULL || iTableNumber == 0 || iNumberOfSeats == 0) {

        return ERROR;

    } else if (IsValidDate(iDay,iMonth,iYear) == 0){

        return ERROR;

    } else {

        RESERVATION *pThis = CreateReservation(iTableNumber, iNumberOfSeats, iDay, iMonth, iYear, iHour, iMinutes,
                                               acGuestName);
        if (pThis == NULL) {
            printf("Failed when creating reservation.\n");
            return ERROR;
        } else {

            // List is empty
            if (pList->pHead == NULL) {

                pThis->pNextReservation = NULL;
                pThis->pPrevReservation = NULL;
                pList->pHead = pThis;
                pList->pTail = pThis;

                return SUCCESS;

            } else{
                //If the list is not empty. That means we have at least one reservation to compare.
                RESERVATION *pCurrent = pList->pHead;
                RESERVATION *pPrev = NULL;


                while (pCurrent != NULL && strcmp(pThis->acGuestName, pCurrent->acGuestName) > 0) {

                    //This will be the previous reservation
                    pPrev = pCurrent;
                    //This will be the next reservation
                    pCurrent = pCurrent->pNextReservation;

                }

                pThis->pNextReservation = pCurrent;
                pThis->pPrevReservation = pPrev;

                if (pPrev != NULL) { //If It's null, we are inserting at spot number 1 (head).
                    pPrev->pNextReservation = pThis;
                } else {
                    pList->pHead = pThis;
                }

                if (pCurrent != NULL) { //Same as with head, but now with tail.
                    pCurrent->pPrevReservation = pThis;
                } else {
                    pList->pTail = pThis;
                }

                return SUCCESS;

            }
        }
    }
}

// Find a reservation based on the guest name.
RESERVATION *FindReservationByName(RESERVATION_LIST *pList, char *pszGuestName) {

    char acGuestNameToLower[MAX_NAME_LENGTH];

    if (pList == NULL || pszGuestName == NULL) {
        return NULL;
    } else{

        //When comparing, I'm calling to lower on both the input and the reservation.
        //Didn't want to manipulate the original name, so I'm creating a copy.
        ToLower(pszGuestName,acGuestNameToLower);

        RESERVATION *pReservation = pList->pHead;

        while (pReservation != NULL) {

            if (strcmp(pReservation->acGuestName, acGuestNameToLower) == 0) {
                return pReservation;
            }
            pReservation = pReservation->pNextReservation;
        }

        return NULL;
    }
}

//Takes in an index to search for reservations.
RESERVATION *FindReservationByIndex(RESERVATION_LIST *pList, int iReservationNumberOrdered) {

    int iCount = 1;

    //We return early if index i not > 0
    if (pList == NULL || iReservationNumberOrdered <= 0) {
        return NULL;
    } else{

        RESERVATION *pReservation = pList->pHead;
        while (pReservation != NULL) {
            if (iCount == iReservationNumberOrdered) {
                return pReservation;
            }
            pReservation = pReservation->pNextReservation;
            iCount++;
        }

        return NULL;
    }
}

// Validate a reservation.
int ValidateReservation(const RESERVATION *pReservation) {

    if (pReservation == NULL) {
        return 1; // Reservation is NULL
    }

    if (pReservation->iTableNumber <= 0) {
        return 2; // Invalid table number
    }

    if (pReservation->iNumberOfSeats <= 0) {
        return 3; // Invalid number of seats
    }

    if (pReservation->iHour <= 0) {
        return 4; // Invalid time
    }

    if (pReservation->iMinute < 0) {
        return 4; // Invalid time
    }

    if (pReservation->acGuestName[0] == '\0') {
        return 5; // Guest name is empty
    }

    return 0; // Reservation is valid
}

int ValidNumber(int iInput, int iRangeStart,int iRangeEnd){

    if(iInput < iRangeStart || iInput >= iRangeEnd){
        return 0;
    }

    return 1;
}

// Function to delete an order list.
void DeleteOrderList(ORDER *pOrder) {
    if (pOrder == NULL || pOrder->next == NULL) {
        return; // No list or list is empty, nothing to delete
    } else{
        ORDER *current = pOrder->next;
        ORDER *next;

        while (current != NULL) {
            next = current->next;
            free(current); // Free memory for the current order
            current = next;
        }

        pOrder->next = NULL;
    }
}


// Delete reservation based on guest name.
int DeleteReservationBasedOnGuestName(RESERVATION_LIST *pList, char *pszGuestName) {

    char szGuestNameToLower[MAX_NAME_LENGTH];

    // Checking if list does not exist or if list is empty.
    if (pList == NULL || pList->pHead == NULL || pszGuestName == NULL) {
        return ERROR;
    } else{

        ToLower(pszGuestName,szGuestNameToLower);

        RESERVATION *pCurrent = pList->pHead;

        while (pCurrent != NULL) {

            if (strcmp(pCurrent->acGuestName, szGuestNameToLower) == 0) {
                // Delete orders associated with the reservation
                if(pCurrent->pOrder != NULL){
                    DeleteOrderList(pCurrent->pOrder);
                }
                // Deleting head
                if (pCurrent->pPrevReservation == NULL) {
                    if (pCurrent->pNextReservation == NULL) {
                        // pCurrent is the only reservation in the list.
                        pList->pTail = NULL;
                        pList->pHead = NULL;
                    } else {
                        pCurrent->pNextReservation->pPrevReservation = NULL;
                        pList->pHead = pCurrent->pNextReservation;
                    }
                } else if (pCurrent->pNextReservation == NULL) {
                    // Deleting tail
                    pCurrent->pPrevReservation->pNextReservation = NULL;
                    pList->pTail = pCurrent->pPrevReservation;
                } else {
                    // Deleting somewhere in the middle
                    pCurrent->pPrevReservation->pNextReservation = pCurrent->pNextReservation;
                    pCurrent->pNextReservation->pPrevReservation = pCurrent->pPrevReservation;
                }

                // Free memory and return success
                free(pCurrent);
                return SUCCESS;
            }

            pCurrent = pCurrent->pNextReservation;
        }
        return ERROR; // Guest name not found
    }
}

// Function to find a reservation based on the table number.
RESERVATION *FindReservation(RESERVATION_LIST *pList, int iTableNumber) {
    if (pList == NULL || iTableNumber <= 0) {
        return NULL; // Invalid input, return NULL
    } else{

        RESERVATION *pReservation = pList->pHead;

        while (pReservation != NULL) {
            if (pReservation->iTableNumber == iTableNumber) {
                return pReservation; // Found reservation with matching table number
            }
            pReservation = pReservation->pNextReservation;
        }

        return NULL; // Reservation not found
    }
}

//A function that either prints a receipt paying together or separate
//Function takes a flag to decide if it's going to print total price for table
//or per person.
void PrintReceipt(RESERVATION *pReservation, int iPayTogether) {

    if (pReservation == NULL) {
        printf("No reservation to print a receipt for.\n");
        return;
    } else{

        // Print reservation details
        printf("Receipt for Table Number: %d\n", pReservation->iTableNumber);
        printf("Number of Seats: %d\n", pReservation->iNumberOfSeats);
        printf("Date: %02d-%02d-%04d\n", pReservation->iDay, pReservation->iMonth, pReservation->iYear);
        printf("Time: %02d:%02d\n", pReservation->iHour, pReservation->iMinute);
        printf("Reservation Name: %s\n", pReservation->acGuestName);

        // Print orders associated with this reservation
        printf("Orders:\n");
        if(iPayTogether == 1){
            ViewAllOrders(pReservation->pOrder);
            int totalCost = CalculateTotalCost(pReservation->pOrder);
            printf("Total Cost: %d\n", totalCost);
        } else{
            PrintOrdersPerGuest(pReservation->pOrder);
        }
    }
}

// Function to list one reservation based on table number.
void ListReservationByTableNumber(RESERVATION_LIST *pList, int iTableNumberOrdered) {

    int count = 1;

    if (pList == NULL || pList->pHead == NULL) {
        printf("No reservations to show\r\n");
        return;
    } else{

        RESERVATION *pCurrent = pList->pHead;

        while (pCurrent != NULL) {
            if (count == iTableNumberOrdered) {
                printf("Table Number: %d\r\n", pCurrent->iTableNumber);
                printf("Number of Seats: %d\r\n", pCurrent->iNumberOfSeats);
                printf("Date: %02d-%02d-%04d\n", pCurrent->iDay, pCurrent->iMonth, pCurrent->iYear);
                printf("Time: %02d:%02d\n", pCurrent->iHour, pCurrent->iMinute);
                printf("Guest Name: %s\r\n", pCurrent->acGuestName);

                // Print orders associated with this reservation
                printf("Orders for this reservation:\r\n");
                ViewAllOrders(pCurrent->pOrder);

                return; // Found and listed reservation, exit function
            }
            count++;
            pCurrent = pCurrent->pNextReservation;
        }

        printf("No table found at place number %d\r\n", iTableNumberOrdered);
    }
}

// Function to print a reservation and all its orders.
void PrintReservationAndOrders(RESERVATION *pReservation) {

    if (pReservation == NULL) {
        printf("No reservation to show.\n");
        return;
    } else{
        // Print reservation details
        printf("Table Number: %d\n", pReservation->iTableNumber);
        printf("Number of Seats: %d\n", pReservation->iNumberOfSeats);
        printf("Date: %02d-%02d-%04d\n", pReservation->iDay, pReservation->iMonth, pReservation->iYear);
        printf("Time: %02d:%02d\n", pReservation->iHour, pReservation->iMinute);
        printf("Guest Name: %s\n", pReservation->acGuestName);

        // Check if there are orders associated with this reservation
        if (pReservation->pOrder != NULL) {
            printf("Orders for this reservation:\n");
            // Assuming ViewAllOrders is a function that prints all orders
            ViewAllOrders(pReservation->pOrder);
        } else {
            printf("No orders placed yet for this reservation.\n");
        }
    }
}

// Function to print a reservation.
void PrintReservationDetails(RESERVATION *pReservation) {
    if (pReservation == NULL) {
        printf("No reservation to show.\n");
        return;
    } else{
        // Print reservation details
        printf("Reservation name: %s\n", pReservation->acGuestName);
        printf("Table number: %d\n", pReservation->iTableNumber);
        printf("Date: %02d-%02d-%04d\n", pReservation->iDay, pReservation->iMonth, pReservation->iYear);
        printf("Time: %02d:%02d\n", pReservation->iHour, pReservation->iMinute);
    }
}

// Deletes all reservations and their associated orders.
int DeleteReservations(RESERVATION_LIST *pList) {
    if (pList == NULL || pList->pHead == NULL) {
        return ERROR; // No reservations to delete
    } else{

        RESERVATION *pCurrent = pList->pHead;
        RESERVATION *pTemp;

        while (pCurrent != NULL) {
            pTemp = pCurrent;
            pCurrent = pCurrent->pNextReservation;

            // Delete orders associated with the reservation
            DeleteOrders(pTemp->pOrder);

            free(pTemp);
        }

        pList->pHead = NULL;
        pList->pTail = NULL;

        return SUCCESS;
    }
}





