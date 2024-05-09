#include "stdio.h"
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#include "./include/oppgave5_network_defs.h"
#include "./include/oppgave5_valid_address.h"
#include "./include/oppgave5_server.h"

static void *HandleClient(void *arg) {

    int client_socket_FD = *((int *)arg);

    int iBytesReceived = 0;
    int isCorrectProtocol = 0;
    int iMagicNumber = 0;
    int iHeaderRead = 0;
    int iTotalRecBytes = 0;
    int iMessageLength = 0;
    int iPhone = 0;

    char buffer[BUFFER_SIZE] = {0};
    char *pszClientMessage = NULL;
    char *pszCurrentPosition = NULL;

    struct TASK5_CONNECT_MESSAGE sClientInfo = {0};
    struct sockaddr_in saAddr = {0};
    ACCEPT_MESSAGE sAcceptMessage = {0};
    sAcceptMessage.iAcceptFlag = 0;

    //Reading connect message
    while (1) {

        iBytesReceived = recv(client_socket_FD, &sClientInfo, sizeof(struct TASK5_CONNECT_MESSAGE), MSG_DONTWAIT);

        if (iBytesReceived < 0) {

            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                usleep(100000);
                continue;
            } else {
                printf("Failed with error %i\n", errno);
                break;
            }
        }

        //Reading content length
        if(iBytesReceived != sizeof (struct TASK5_CONNECT_MESSAGE)) {
            send(client_socket_FD,&sAcceptMessage,sizeof (ACCEPT_MESSAGE),0);
            printf("Unknown welcome message format. Closing connection.\n");
            break;
        }else{//Check magic number, so we now we are on right protocol.
            if(sClientInfo.iMagicNumber != MAGIC_NUMBER){
                printf("Unknown magic number. Closing connection.\n");
                send(client_socket_FD,&sAcceptMessage,sizeof (ACCEPT_MESSAGE),0);
                break;
            } else{
                saAddr.sin_addr.s_addr = sClientInfo.iIpAddress;
                if(!isValidAddress(inet_ntoa(saAddr.sin_addr))){
                    printf("Unknown ip address. Closing connection.\n");
                    send(client_socket_FD,&sAcceptMessage,sizeof (ACCEPT_MESSAGE),0);
                } else{
                    //If we arrive here, we are ready for sending accept message back.
                    //We also summarize on server console which client that joined.
                    printf("----Socket Number %d----\n", client_socket_FD);
                    printf("Magic Number: %d\n", sClientInfo.iMagicNumber);
                    printf("Client IP: %s\n", inet_ntoa(saAddr.sin_addr));
                    printf("Client Phone: %d\n\n", sClientInfo.iPhoneNumber);

                    sAcceptMessage.iAcceptFlag = 1;
                    iPhone = sClientInfo.iPhoneNumber;
                    send(client_socket_FD,&sAcceptMessage,sizeof (ACCEPT_MESSAGE),0);
                    isCorrectProtocol = 1;
                    break;
                }
            }
        }
    }

    //The server will here wait for message from client.
    //The server will always check header for length and magic number. Then it sets iReadheader to 1.
    //After that the server read bytes until it reaches content length.

    while (isCorrectProtocol == 1){

        memset(buffer,0,BUFFER_SIZE);

        iBytesReceived = recv(client_socket_FD,buffer,BUFFER_SIZE - 1,MSG_DONTWAIT);

        if (iBytesReceived < 0) {

            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                usleep(100000);
                continue;
            } else {
                printf("Failed to read %i\n",errno);
                break;
            }
        } else{

            if(iBytesReceived > 0){

                iTotalRecBytes += iBytesReceived;

                buffer[iBytesReceived] = '\0';

                char *pTemp = buffer;
                //Check if we have read header.
                if(iHeaderRead != 1){

                    memcpy(&iMagicNumber,pTemp,sizeof (int ));

                    if(iMagicNumber == sClientInfo.iMagicNumber){
                        pTemp += 4;
                        //We now move 1 int forward to read the message length,
                        //and after that we malloc pszClientMessage so we can start reading the message.
                        memcpy(&iMessageLength,pTemp,sizeof (int ));
                        if(iMessageLength > 0){
                            pszClientMessage = (char*) malloc(iMessageLength + 1);
                            if(pszClientMessage == NULL){
                                break;
                            }
                            pszCurrentPosition = pszClientMessage;
                            iHeaderRead = 1;
                        } else{
                            printf("No content length provided.\n");
                            break;
                        }
                    } else{
                        printf("Unknown magic number.\n");
                        break;
                    }

                }

                //We will arrive here as long as the header is read and its more bytes to read.
                if (iHeaderRead == 1 && iTotalRecBytes <= iMessageLength) {

                    memcpy(pszCurrentPosition, buffer, iBytesReceived);
                    pszCurrentPosition += iBytesReceived; // Move the current position pointer
                    // We have received the entire message
                    if (iTotalRecBytes == iMessageLength) {

                        //pszClientMessage should now contain the entire message
                        //so we null terminate the message.
                        pszClientMessage[iMessageLength] = '\0'; // Null-terminate the message
                        char *pszMessageContent = pszClientMessage + 8; // Skip the header
                        if(strncmp(pszMessageContent,"quit",4) == 0){
                            printf("Bye, bye! Shutting down socket nr %d\n",client_socket_FD);
                            break;
                        }
                        printf("\nNumber %d: %s\n",iPhone, pszMessageContent);

                        //Now that we got everything from the current transaction
                        //We reset iHeaderRead and iTotalRecBytes
                        iHeaderRead = 0;
                        iTotalRecBytes = 0;
                        //We also free the memory
                        free(pszClientMessage);
                        pszClientMessage = NULL;
                    }
                }
            }

        }
    }

    printf("Thread finished\n");

    if(pszClientMessage != NULL){
        free(pszClientMessage);
    }
    close(client_socket_FD); // Close the client socket

    pthread_exit(NULL);
}

void StartServer(int iPort, char *psIpAddress){

    int sockFd = -1;
    struct sockaddr_in saAddr = {0}, saClientAddr = {0};
    socklen_t iAddressLength;

    sockFd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockFd < 0) {
        printf("socket failed with %i", errno);
    } else{

        //Setting up server
        saAddr.sin_family = AF_INET;
        saAddr.sin_port = htons(iPort);
        saAddr.sin_addr.s_addr = inet_addr(psIpAddress);

        //Just to display the address in the server console.
        char originalIpAddress[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(saAddr.sin_addr), originalIpAddress, INET_ADDRSTRLEN);

        if (bind(sockFd, (struct sockaddr *)&saAddr, sizeof(saAddr)) < 0) {
            printf("bind failed with %i", errno);
        } else{
            if(listen(sockFd,5) < 0){
                printf("listen failed with %i", errno);
            } else{

                int clients = 0;
                pthread_t pThreads[MAX_CLIENTS];

                system("clear");
                printf("----Server Started up----\n");
                printf("Port number: %d\n", iPort);
                printf("IP Address: %s\n", originalIpAddress);
                printf("-------------------------\n");
                printf("Clients are now welcome!\n");
                printf("-------------------------\n");

                //We listen for clients until MAX_CLIENTS are reached.
                while (clients < MAX_CLIENTS){

                    iAddressLength = sizeof(saClientAddr);
                    int client_socket_FD = accept(sockFd, (struct sockaddr *)&saClientAddr, &iAddressLength);
                    if (client_socket_FD < 0) {
                        printf("Failed on accept %i", errno);
                        continue;
                    }

                    //Client has connected and we spin up a thread and pass inn the client_socket
                    if(pthread_create(&pThreads[clients],NULL,HandleClient,(void *) &client_socket_FD) != 0){
                        printf("Failed on accept %i", errno);
                        close(client_socket_FD);
                        continue;
                    }

                    //We increase client count so we arrive at pthread_join
                    clients++;
                }
                //We arrive here when both clients have messaged the server quit
                for (int i = 0; i < MAX_CLIENTS; i++) {
                    pthread_join(pThreads[i], NULL);
                }
            }//listen
        }//Bind
    } // Socket

    if(sockFd != -1){
        close(sockFd);
    }
}


