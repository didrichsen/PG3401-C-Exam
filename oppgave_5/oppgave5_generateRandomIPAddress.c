#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "./include/oppgave5_generateRandomIPAddress.h"

//Generating a random ip address as client info 

void GenerateRandomIPAddress(char *buffer) {

    srand(time(NULL));

    int part1 = rand() % (MAX_IP_PART + 1);
    int part2 = rand() % (MAX_IP_PART + 1);
    int part3 = rand() % (MAX_IP_PART + 1);
    int part4 = rand() % (MAX_IP_PART + 1);

    sprintf(buffer, "%d.%d.%d.%d", part1, part2, part3, part4);
}