/* main.c header file ----------------------------------------------------------------------

 Project: Exam PG3401 V24 - Oppgave 5
 Kandidatnummer : 38
 Description: Main source file for Oppgave 5

 The program can start either a server or a client. When a server is started
 up it accepts up to two clients, giving each client a seperate thread with its
 own socket. Clients that connect sends a welcome message, when the server sends an
 accept message back, the clients can write to the server and the server displays the
 messages in the server console. When client writes 'quit', det threads terminates and
 the socket closes. When both clients have ended their session, det server shuts down.

 Executable (server) can be run: ./oppgave_5 -listen -port <portnr>
 Executable (client) can be run: ./oppgave_5 -server -port <same as server> -ip 127.0.0.1 -phone <4 digest +>
--------------------------------------------------------------------------------------------*/

/*=== Standard C library: ===============================================================
=======================================================================================*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*=== Project specific include files: ===================================================
=======================================================================================*/

#include "./include/oppgave5_server.h"
#include "./include/oppgave5_client.h"
#include "./include/oppgave5_valid_address.h"

#define IP_ADDRESS 16

int main(int argc, char* argv[]) {

    if (argc < 4) {
        printf("Too few arguments.\n");
        return 1;
    }

    //Both client and server will connect to 127.0.0.1
    char *ipAddress = "127.0.0.1";
    char pszClientIpAddress[IP_ADDRESS];
    char caPhone[20];
    int iPhone = 0;
    int iPort = 0;

    //Just to test, but let it stay. Using it later in program.
    if (isValidAddress(ipAddress) == 0) {
        printf("Invalid IP address.\n");
        return 1;
    }

    if (strncmp(argv[2], "-port", strlen("-port")) == 0) {
        iPort = atoi(argv[3]);
        if(iPort == 0){
            printf("Choose a valid port number\n and start program "
                   "./program -server <ip> -port <port> -phone <number>.\n");
            return 1;
        }
    }

    //Since server only requires port, we can start server now
    //Client requires additional checks
    if (strncmp(argv[1], "-listen", strlen("-listen")) == 0) {
        StartServer(iPort,ipAddress);
    } else if (strncmp(argv[1], "-server", strlen("-server")) == 0) {

        if (strncmp(argv[4], "-ip", strlen("-ip")) == 0) {

            strncpy(pszClientIpAddress,argv[5],IP_ADDRESS);
            pszClientIpAddress[IP_ADDRESS] = '\0';

            if(!isValidAddress(pszClientIpAddress)){
                printf("Choose a valid number at least 4 digest and max 20\n and start program ./program -server <ip> -port <port> -phone <number>.\n");
                return 1;
            }
        }

        if (strncmp(argv[6], "-phone", strlen("-phone")) == 0) {
            //Copy over and check length.
            //caPhone takes only 20 chars.
            strncpy(caPhone, argv[7],20);
            if(strlen(caPhone) < 4){
                printf("Choose a valid number at least 4 digest and max 20\n and start program ./program -server <ip> -port <port> -phone <number>.\n");
                return 1;
            } else{
                //Converting to "phone" number. 
                iPhone = atoi(argv[7]);
            }
        }

        printf("Started client at port %d with ip address %s and phone %d\n", iPort, argv[3], iPhone);
        StartClient(iPort,pszClientIpAddress,iPhone);
    } else {
        printf("Invalid argument. Use 'server' or 'client'.\n");
        return 1;
    }

    return 0;
}
