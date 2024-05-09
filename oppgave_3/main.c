/* main.c header file ----------------------------------------------------------------------

 Project: Exam PG3401 V24 - Oppgave 3
 Kandidatnummer : 38
 Description: Main source file for Oppgave 3

The following code compiles by writing 'make', creating the executable "oppgave_3".
The program lets you manage reservations and orders for guests at a restaurant.
You can create reservations that are stored under a guest's name,
and orders can be added to the reservation, with each order having a guest's name.
Reservations and orders can be searched and deleted, and receipts can be printed out,
 either separate or together.

 Executable can be run by writing: ./oppgave_3
--------------------------------------------------------------------------------------------*/

/*=== Standard C library: ===============================================================
=======================================================================================*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

/*=== Project specific include files: ===================================================
=======================================================================================*/

#include "./include/oppgave3_menu.h"
#include "./include/oppgave3_reservation.h"
#include "./include/oppgave3_ui.h"
#include "./include/oppgave3_order.h"

int main(int argc, char* argv[]) {

    //Used in switch case to determine operation.
    enum MENU_OPTIONS eMenuOptions;

    //Reservation list keeps track of head and tail
    RESERVATION_LIST sReservationList;
    sReservationList.pHead = NULL;
    sReservationList.pTail = NULL;

    RESERVATION *pReservation = NULL;

    //Used to store the name of the guest who booked the reservation, MAX_NAME_LENGTH 256 characters.
    char szReservationName[MAX_NAME_LENGTH] = {0};
    //Buffer used for different user interactions. BUFFER_SIZE 256 characters.
    char pszBuffer[BUFFER_SIZE] = {0};

    char pszDish[DISH_LENGTH] = {0};

    char pszFirstNameGuest[MAX_NAME_LENGTH] = {0};

    int iTableNumber = 0, iHours = 0, iMinutes = 0, iPrice = 0, iNumberOfGuest = 0;
    int iDay = 0, iMonth = 0,iYear = 0;
    //Return code to check for errors
    int iRc = 0;
    //Variables used later in code to determine min og max table number.
    int iMinTableNumberRestaurant = 1, iMaxTableNumberRestaurant = 50;

    //Variables to set min and max guests to be booked at through one reservation.
    int iMinNumberOfGuestsPerReservation = 1, iMaxNumberOfGuestsPerReservation = 8;

    do {

        pReservation = NULL;

        //Prompts user for which option to pick.
        eMenuOptions = GetMenuOption();

        switch (eMenuOptions) {
            case MENU_OPTION_ADD_RESERVATION:

                do {
                    do {
                        //Getting the name of the person putting in the reservation
                        memset(szReservationName, 0, MAX_NAME_LENGTH);
                        printf("Enter reservation name: \n");
                        GetZeroTerminatedUserInput(szReservationName, MAX_NAME_LENGTH);

                    } while (!IsValidInput(szReservationName,1));


                    do {
                        //Getting the table number
                        memset(pszBuffer, 0, BUFFER_SIZE);
                        iTableNumber = 0;

                        printf("Enter table number: \n");

                        GetZeroTerminatedUserInput(pszBuffer, BUFFER_SIZE);
                        iTableNumber = atoi(pszBuffer);

                        //Check that we stay within the range of the available tables at the restaurant.
                        if (!ValidNumber(iTableNumber,iMinTableNumberRestaurant,iMaxTableNumberRestaurant)) {
                            printf("\x1b[31m"); //Prints red text
                            printf("Please stay within our range of valid table numbers, %d -> %d\n", iMinTableNumberRestaurant,iMaxTableNumberRestaurant);
                            printf("\x1b[0m");
                        }

                    } while (!ValidNumber(iTableNumber,iMinTableNumberRestaurant,iMaxTableNumberRestaurant));

                    //Getting number of guests number
                    do {
                        memset(pszBuffer, 0, BUFFER_SIZE);
                        iNumberOfGuest = 0;

                        printf("Enter number of guests: \n");

                        GetZeroTerminatedUserInput(pszBuffer, BUFFER_SIZE);
                        iNumberOfGuest = atoi(pszBuffer);

                        //Check if the number of guests are within the range that can be booked without talking to manager.
                        if (!ValidNumber(iNumberOfGuest,iMinNumberOfGuestsPerReservation,iMaxNumberOfGuestsPerReservation)) {
                            printf("\x1b[31m"); //Prints red text
                            printf("Guests per order has to stay within range of, %d - %d\n", iMinNumberOfGuestsPerReservation, iMaxNumberOfGuestsPerReservation);
                            printf("Above %d has to be booked through the manager.\n", iMaxNumberOfGuestsPerReservation);
                            printf("\x1b[0m");
                        }

                    } while (!ValidNumber(iNumberOfGuest,iMinNumberOfGuestsPerReservation,iMaxNumberOfGuestsPerReservation));

                do{

                    do {

                    // Get user input for hours
                    memset(pszBuffer,0,BUFFER_SIZE);
                    iHours = 0;
                    printf("Enter time of reservation\n");
                    printf("Enter hour (24 hours clock): \n");
                    GetZeroTerminatedUserInput(pszBuffer,BUFFER_SIZE);

                    } while (!IsValidInput(pszBuffer,2));

                    iHours = atoi(pszBuffer);

                    if(iHours == 0){ //If atoi fails and return 0, we set hour to be default 17.
                        iHours = 17;
                    }

                    do{
                        // Get user input for minutes
                        memset(pszBuffer,0,BUFFER_SIZE);
                        iMinutes = 0;
                        printf("Enter minutes (24 hours clock): \n");
                        GetZeroTerminatedUserInput(pszBuffer,BUFFER_SIZE);

                    } while (!IsValidInput(pszBuffer,2));

                    iMinutes = atoi(pszBuffer);

                    // Validate input and prompt user to enter again if necessary
                    if (iHours < 10 || iHours > 23 || iMinutes < 0 || iMinutes >= 60) {
                        printf("\x1b[31m"); // Prints red text
                        printf("Invalid time. Please enter a valid time.\n");
                        printf("Restaurant is open between 10:00-23:00.\n");
                        printf("\x1b[0m");
                    }

                } while (iHours < 10 || iHours > 23 || iMinutes < 0 || iMinutes >= 60);

                do {

                    do {
                        memset(pszBuffer, 0, BUFFER_SIZE);
                        printf("Enter day (DD): \n");
                        GetZeroTerminatedUserInput(pszBuffer, BUFFER_SIZE);
                    } while (!IsValidInput(pszBuffer, 2));

                    iDay = atoi(pszBuffer);

                    do {
                        memset(pszBuffer, 0, BUFFER_SIZE);
                        printf("Enter month (MM): \n");
                        GetZeroTerminatedUserInput(pszBuffer, BUFFER_SIZE);
                    } while (!IsValidInput(pszBuffer, 2));

                    iMonth = atoi(pszBuffer);

                    do {
                        memset(pszBuffer, 0, BUFFER_SIZE);
                        printf("Enter year (YYYY): \n");
                        GetZeroTerminatedUserInput(pszBuffer, BUFFER_SIZE);
                    } while (!IsValidInput(pszBuffer, 2));

                    iYear = atoi(pszBuffer);

                    // Validate input date and prompt user to enter again if needed.
                    if (!IsValidDate(iDay, iMonth, iYear)) {
                        printf("\x1b[31m"); // Prints red text
                        printf("Invalid date. Please enter a valid date.\n");
                        printf("Day 1-31, Month 1-12, Year >= 2024.\n");
                        printf("\x1b[0m");
                    }
                } while (!IsValidDate(iDay, iMonth, iYear));

                    //We summarize reservation details and give the user the option to redo if needed.
                    printf("*************************\n");
                    printf("*  Reservation Summary  *\n");
                    printf("*************************\n");
                    printf("Reserved by: %s\n", szReservationName);
                    printf("Table Number: %d\n", iTableNumber);
                    printf("Number of Guests: %d\n", iNumberOfGuest);
                    printf("Date: %02d-%02d-%04d\n", iDay, iMonth, iYear);
                    printf("Time: %02d:%02d\n", iHours, iMinutes);
                    printf("Save reservation (y) or edit (n)?\n");

                    memset(pszBuffer, 0, BUFFER_SIZE);

                    GetZeroTerminatedUserInput(pszBuffer, BUFFER_SIZE);

                } while (pszBuffer[0] != 'y');


                //At this point we got everything we need to create and insert a reservation.
                iRc = InsertReservation(&sReservationList,iTableNumber,iNumberOfGuest,iDay,iMonth,iYear,iHours,iMinutes,szReservationName);

                if(iRc == SUCCESS){
                    printf("\x1b[32m"); //Green success text
                    printf("Reservation created:\r\n");
                    printf("\x1b[0m");
                    printf("Reserved by: %s\r\n", szReservationName);
                    printf("Table Number: %d\r\n", iTableNumber);
                    printf("Number of Guests: %d\r\n", iNumberOfGuest);
                    printf("Date: %02d-%02d-%04d\n", iDay, iMonth, iYear);
                    printf("Time: %02d:%02d\n", iHours, iMinutes);
                } else if(iRc == ERROR){
                    printf("\x1b[31m"); //Prints red text
                    printf("Failed to add reservation to list.\r\n");
                    printf("\x1b[0m");
                }

                ToMainMenu();

                break;

            case MENU_OPTION_GET_RESERVATION:

                do {

                memset(pszBuffer,0,BUFFER_SIZE);
                printf("Enter index to find and print a reservation from an alphabetical ordered list: \n");
                GetZeroTerminatedUserInput(pszBuffer,5);
                } while (!IsValidInput(pszBuffer,2));

                int number = atoi(pszBuffer);

                pReservation = NULL;

                pReservation = FindReservationByIndex(&sReservationList,number);

                if(pReservation == NULL){
                    printf("Couldn't find reservation\n");
                } else{
                    PrintReservationAndOrders(pReservation);
                }

                break;

            case MENU_OPTION_SEARCH_RESERVATION:

                do {

                memset(pszBuffer,0,BUFFER_SIZE);
                printf("Enter reservation name  \n");
                GetZeroTerminatedUserInput(pszBuffer,MAX_NAME_LENGTH);
                } while (!IsValidInput(pszBuffer,1));


                pReservation = NULL;

                pReservation = FindReservationByName(&sReservationList,pszBuffer);

                if(pReservation == NULL){
                    printf("Couldn't find reservation\n");
                } else{
                    PrintReservationDetails(pReservation);
                }

                break;

            case MENU_OPTION_DELETE_RESERVATION:

                do {

                //Making sure reservation name is set to null
                memset(szReservationName,0,MAX_NAME_LENGTH);
                printf("Enter reservation name to delete reservation: \n");
                GetZeroTerminatedUserInput(szReservationName,MAX_NAME_LENGTH);
                } while (!IsValidInput(szReservationName,1));

                iRc = DeleteReservationBasedOnGuestName(&sReservationList,szReservationName);

                if(iRc == ERROR){
                    printf("\x1b[31m"); //Prints red text
                    printf("Failed to delete reservation. Couldn't find reservation name.\r\n");
                    printf("\x1b[0m");
                } else{
                    printf("\x1b[32m");
                    printf("Successfully deleted reservation with reservation name %s\r\n", szReservationName);
                    printf("\x1b[0m");
                }

                ToMainMenu();

                break;

            case MENU_OPTION_ADD_FOOD_AND_BEVERAGE:

                pReservation = NULL;

                do {
                //Getting the name of the reservation
                memset(szReservationName, 0, MAX_NAME_LENGTH);
                printf("Enter name of reservation: \n");
                GetZeroTerminatedUserInput(szReservationName, MAX_NAME_LENGTH);
                } while (!IsValidInput(szReservationName,1));

                pReservation = FindReservationByName(&sReservationList,szReservationName);

                if(pReservation == NULL){
                    printf("\x1b[31m"); //Prints red text
                    printf("Couldn't find reservation.\n");
                    printf("\x1b[0m");
                    break;
                } else {

                    do {

                        //Getting the name of the dish
                        //Here I can later implement some enum connected to price.
                        //Pick from list
                        do {
                            memset(pszDish, 0, DISH_LENGTH);
                            printf("Enter Dish or Drink\n");
                            GetZeroTerminatedUserInput(pszDish, DISH_LENGTH);
                        } while (!IsValidInput(pszDish, 1));

                        do {
                            memset(pszBuffer, 0, BUFFER_SIZE);
                            iPrice = 0;
                            printf("Enter total price: \n");
                            GetZeroTerminatedUserInput(pszBuffer, BUFFER_SIZE);
                        } while (!IsValidInput(pszBuffer, 2));

                        iPrice = atoi(pszBuffer);

                        if (iPrice > 0) {

                            do {
                                //Getting the name of the guest
                                memset(pszFirstNameGuest, 0, MAX_NAME_LENGTH);
                                printf("Enter Guest Name\n");
                                GetZeroTerminatedUserInput(pszFirstNameGuest, MAX_NAME_LENGTH);
                            } while (!IsValidInput(pszFirstNameGuest, 1));

                            iRc = AddOrder(&(pReservation->pOrder), pszFirstNameGuest, iPrice, pszDish);

                            if (iRc == ERROR) {
                                printf("Couldn't add order to reservation.\r\n");
                                break;
                            } else {
                                printf("\x1b[32m");
                                printf("Successfully");
                                printf("\x1b[0m");
                                printf(" added \x1b[35m%s\x1b[0m, for guest, \x1b[35m%s\x1b[0m.", pszDish,
                                       pszFirstNameGuest);
                                printf("\r\n");
                            }

                        } else {
                            printf("Price needs to be added\n");
                        }


                        do {
                            memset(pszBuffer, 0, BUFFER_SIZE);
                            printf("Add another order? (y or n): \n");
                            GetZeroTerminatedUserInput(pszBuffer, BUFFER_SIZE);
                        } while (!IsValidInput(pszBuffer, 1));

                    } while (pszBuffer[0] == 'y');
                }

                ToMainMenu();

                break;
            case MENU_OPTION_PRINT_TABLE_ORDERS:

                do{
                    //Getting the name of the reservation
                    memset(szReservationName, 0, MAX_NAME_LENGTH);
                    printf("Enter name of reservation: \n");
                    GetZeroTerminatedUserInput(szReservationName, MAX_NAME_LENGTH);
                } while (!IsValidInput(szReservationName,1));

                pReservation = NULL;

                pReservation = FindReservationByName(&sReservationList,szReservationName);

                if(pReservation == NULL){
                    printf("\x1b[31m"); //Prints red text
                    printf("Couldn't find reservation.\n");
                    printf("\x1b[0m");
                    break;
                } else{
                    PrintReceipt(pReservation,1);
                }

                ToMainMenu();

                break;
                case MENU_OPTION_PRINT_ORDER_BY_GUEST:

                    do{
                        //Getting the name of the reservation
                        memset(szReservationName, 0, MAX_NAME_LENGTH);
                        printf("Enter name of reservation: \n");
                        GetZeroTerminatedUserInput(szReservationName, MAX_NAME_LENGTH);
                    } while (!IsValidInput(szReservationName,1));

                pReservation = NULL;

                pReservation = FindReservationByName(&sReservationList,szReservationName);

                if(pReservation == NULL){
                    printf("\x1b[31m"); //Prints red text
                    printf("Couldn't find reservation.\n");
                    printf("\x1b[0m");
                    break;
                } else{
                    PrintReceipt(pReservation,0);
                }

                ToMainMenu();

                break;

            case MENU_OPTION_PRINT_GUESTS_ORDERS:

                do{
                    //Getting the name of the reservation
                    memset(szReservationName, 0, MAX_NAME_LENGTH);
                    printf("Enter name of reservation: \n");
                    GetZeroTerminatedUserInput(szReservationName, MAX_NAME_LENGTH);
                } while (!IsValidInput(szReservationName,1));

                pReservation = NULL;

                pReservation = FindReservationByName(&sReservationList,szReservationName);

                if(pReservation == NULL){
                    printf("\x1b[31m"); //Prints red text
                    printf("Couldn't find reservation.\n");
                    printf("\x1b[0m");
                    break;
                } else{

                    do{
                        //Getting the name of one of the persons at the reservation to show his/hers orders.
                        memset(pszFirstNameGuest, 0, MAX_NAME_LENGTH);
                        printf("-- Enter guests name to print order --\n");
                        GetZeroTerminatedUserInput(pszFirstNameGuest, MAX_NAME_LENGTH);
                        /*
                        iRc = PrintGuestNames(pReservation->pOrder);
                        if(iRc == ERROR){
                            break;
                        } else{
                            GetZeroTerminatedUserInput(pszFirstNameGuest, MAX_NAME_LENGTH);
                        }
                         */
                    } while (!IsValidInput(szReservationName,1));

                    if(iRc != ERROR){
                        PrintGuestOrders(pReservation->pOrder,pszFirstNameGuest);
                    }
                }

                break;

            case MENU_OPTION_EXIT:
                // Code to handle exit
                ExitProgram();
                break;
            default:
                // Code to handle invalid option
                printf("Invalid option selected.\n");
                usleep(2000000);
                break;
        }

    } while (eMenuOptions != MENU_OPTION_EXIT);

    if(sReservationList.pHead != NULL){
        iRc = DeleteReservations(&sReservationList);
    }

    if(iRc == ERROR){
        printf("An error occurred while freeing memory.");
        return 1;
    } else{
        return 0;
    }

}