#include <stdio.h>
#include <unistd.h>

#include "./include/oppgave3_ui.h"

//Used to create a little more engaging console

void ToMainMenu(){
    printf("Returning to main menu .");
    fflush(stdout);
    usleep(750000);

    printf(" .");
    fflush(stdout);
    usleep(750000);

    printf(" .");
    fflush(stdout);
    usleep(750000);

    printf("\r\033[K");
}

void ExitProgram(){
    printf("Cleaning up and exiting .\n");
    fflush(stdout);
    usleep(750000);

    printf(" .");
    fflush(stdout);
    usleep(750000);

    printf(" .");
    fflush(stdout);
    usleep(750000);

    printf("\r\033[K");
}