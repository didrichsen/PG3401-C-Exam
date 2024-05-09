#include <stdio.h>
#include <stdlib.h>

#include "./include/oppgave3_menu.h"
#include "./include/oppgave3_input.h"

//Prints menu.
static void PrintMenu() {
    printf("\x1b[1;36m");
    printf("*************************\n");
    printf("*       MAIN MENU       *\n");
    printf("*************************\n");
    printf("\nPlease select an option:\n");
    printf("> 1 -\tAdd Reservation\n");
    printf("> 2 -\tGet Reservation\n");
    printf("> 3 -\tSearch Reservation\n");
    printf("> 4 -\tDelete Reservation\n");
    printf("> 5 -\tAdd Food And Beverage to Reservation\n");
    printf("> 6 -\tPrint Reservation and Orders\n");
    printf("> 7 -\tPrint Orders per guest for separate recites\n");
    printf("> 8 -\tPrint Guests Orders\n");
    printf("> 9 -\tExit\n");
    printf("*************************\n");
    printf("\x1b[0m");
}

//Makes use of GetZeroTerminatedUserInput but also do a couple of checks that specific to menu option.
static int GetMenuInput(char firstOption, char lastOption){

    char cInput[10] = {0};
    int iChoice = -1;

    PrintMenu();

    do {
        GetZeroTerminatedUserInput(cInput,10);
        //We check [0] which should be within range in the ASCII Table.
        //If [1] is not \0 then the user has inputted higher number than 10.
        if(cInput[0] < firstOption || cInput[0] > lastOption || cInput[1] != '\0') {
            printf("You have to stay within menu length, %c - %c\r\n",firstOption,lastOption);
        }else{
            iChoice = atoi(cInput);
        }
    } while (iChoice == -1);

    return iChoice;
}

//Checks the option return from GetMenuInput and return the corresponding enum value.
enum MENU_OPTIONS GetMenuOption() {
    int option = GetMenuInput('1', '9');

    if (option == 1) {
        return MENU_OPTION_ADD_RESERVATION;
    } else if (option == 2) {
        return MENU_OPTION_GET_RESERVATION;
    } else if (option == 3) {
        return MENU_OPTION_SEARCH_RESERVATION;
    } else if (option == 4) {
        return MENU_OPTION_DELETE_RESERVATION;
    } else if (option == 5) {
        return MENU_OPTION_ADD_FOOD_AND_BEVERAGE;
    } else if (option == 6) {
        return MENU_OPTION_PRINT_TABLE_ORDERS;
    } else if (option == 7) {
        return MENU_OPTION_PRINT_GUESTS_ORDERS;
    }else if (option == 8) {
        return MENU_OPTION_PRINT_ORDER_BY_GUEST;
    } else if (option == 9) {
        return MENU_OPTION_EXIT;
    }
}