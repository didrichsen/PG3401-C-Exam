#include "stdio.h"
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "./include/oppgave5_client.h"
#include "./include/oppgave5_network_defs.h"
#include "./include/oppgave5_generateRandomIPAddress.h"


void StartClient(int iPort, char *psClientIpAddress, int iPhone){

    //Generating a random IP address.
    //Just to fill out fictional client info field on server side.
    char szIpBuffer[17] = {0};

    GenerateRandomIPAddress(szIpBuffer);

    int socketFd = -1;
    int iRc = 0;
    int iStringLength = 0;
    int write_to_server = 0;
    struct sockaddr_in saAddr = {0};

    char pszUserInput[INPUT_SIZE] = {0};

    ACCEPT_MESSAGE sAcceptMessage = {0};
    sAcceptMessage.iAcceptFlag = 0;
    SEND_MESSAGE *sClientMessage = NULL;

    struct TASK5_CONNECT_MESSAGE sConnectMessage;

    socketFd = socket(AF_INET, SOCK_STREAM, 0);

    if (socketFd < 0) {
        //pg3401Error("socket failed with %i", errno);
        printf("socket failed with %i", errno);
    } else {
        //Preparing connect and connect message.
        saAddr.sin_family = AF_INET;
        saAddr.sin_port = htons(iPort);
        saAddr.sin_addr.s_addr = inet_addr(psClientIpAddress);

        sConnectMessage.iIpAddress = inet_addr(szIpBuffer);
        sConnectMessage.iMagicNumber = MAGIC_NUMBER;
        sConnectMessage.iMessageSize = sizeof (struct TASK5_CONNECT_MESSAGE);
        sConnectMessage.iPhoneNumber = iPhone;

        if (connect(socketFd, (struct sockaddr *)&saAddr, sizeof(saAddr)) < 0) {
            printf("Failed to connect %d\n",errno);
        } else{

            send(socketFd,&sConnectMessage,sizeof (struct TASK5_CONNECT_MESSAGE),0);
            //We wait for the accept message from server.
            while (1) {
                iRc = recv(socketFd, &sAcceptMessage, sizeof(ACCEPT_MESSAGE), MSG_DONTWAIT);

                if (iRc == 0) {
                    break;
                } else if (iRc > 0) {
                    //When accepted. We set write_to_server = 1
                    //Will be used in the loop when writing to server.
                    if(sAcceptMessage.iAcceptFlag){
                        printf("Accepted by server. Ready to write.\n");
                        write_to_server = 1;
                    } else{
                        printf("Accept Flag not valid.");
                    }
                    break;
                } else if (errno == EAGAIN || errno == EWOULDBLOCK) {
                    usleep(100000);
                } else {
                    printf("%d\n",errno);
                    break;
                }
            }

            while (write_to_server){

                memset(pszUserInput,0,INPUT_SIZE);

                //Reading into a 1024 bytes buffer which will write to
                //127 bytes server buffer. 
                if (fgets(pszUserInput, INPUT_SIZE - 1, stdin) != NULL) {
                    char *pTemp = pszUserInput;
                    while (*pTemp != '\n' && *pTemp != '\0') {
                        pTemp++;
                    }
                    //Now we can use strlen to find size.
                    *pTemp = '\0';
                }

                //Strlen calculates length up to but not included the zero terminator.
                iStringLength = strlen(pszUserInput);

                sClientMessage = malloc(sizeof (SEND_MESSAGE) + iStringLength + 1); //+ 1 for zero terminator

                sClientMessage->iMagicNumber = MAGIC_NUMBER;
                sClientMessage->iMessageSize = sizeof (sClientMessage) + iStringLength + 1;
                strncpy(sClientMessage->szMessage,pszUserInput,iStringLength);
                //strncpy includes zero terminator if space for it, but we
                //add it to make sure it's there.
                sClientMessage->szMessage[iStringLength] = '\0';
                //Signalize quit to server
                if(strncmp(sClientMessage->szMessage,"quit",4) == 0){
                    send(socketFd,sClientMessage,sClientMessage->iMessageSize,0);
                    break;
                }

                iRc = send(socketFd, sClientMessage, sClientMessage->iMessageSize, 0);

                if (iRc < 0) {
                    printf("Error when sending %d\n", errno);
                    free(sClientMessage);
                    sClientMessage = NULL;
                    break;
                }

                free(sClientMessage);
                sClientMessage = NULL;
            }
        }
    }

    if(sClientMessage != NULL){
        free(sClientMessage);
        sClientMessage = NULL;
    }

    if(socketFd != -1){
        close(socketFd);
    }
}