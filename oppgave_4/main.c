/* main.c header file ----------------------------------------------------------------------

 Project: Exam PG3401 V24 - Oppgave 4
 Kandidatnummer : 38
 Description: Main source file for Oppgave 4

 The program uses two threads, where one threads reads from file and
 the other threads creates a has out of the file. The program terminates
 when the read-thread sets exit to 1, telling hash-thread that all bytes
 are read from file. The hash-thread then creates a hash, which is printed
 as hex to the console. Clean up is done with a function that checks if
 semaphores and mutex are initialized, if so, destroys them and also free
 memory that was allocated for filename.

 Everything is passed to the threads through a struct.

 The executable can be run by writing: ./oppgave_4 eksamen_v24_oppgave4_pg2265.txt
--------------------------------------------------------------------------------------------*/

/*=== Standard C library: ===============================================================
=======================================================================================*/

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*=== Project specific include files: ===================================================
=======================================================================================*/

#include "./include/oppgave4_struct.h"
#include "./include/oppgave4_read_file.h"
#include "./include/oppgave4_create_hash.h"
#include "./include/oppgave4_clean_up.h"

int main(int argc, char* argv[]) {

    int length = 0;
    int iRc = 0;

    // Program requires two arguments: [0] program name and [1] filename
    if (argc < 2) {
        printf("Program requires at least one argument to run. Usage: %s <arg>\n", argv[0]);
        return 1;
    } else{
        // Check length of filename so we can allocate memory and copy filename into struct.
        length = strlen(argv[1]);
        if(length == 0){
            printf("Failed to take string length of argument\n. Can proceed to allocate memory for filename.\n.");
            return 1;
        }
    }

   pthread_t threadA, threadB;
   THREAD_DATA *sThreadData = NULL;

   //Filename is copied into a flexible array. +1 for zero terminator.
   sThreadData = (THREAD_DATA*) malloc(sizeof (THREAD_DATA) + length + 1);

   if(sThreadData == NULL){
       printf("Failed to allocate memory\n");
       return 1;
   } else{

       sThreadData->sha1Ctx = NULL;

       //Initialize sha1 struct so we can pass it to the threads.
       sThreadData->sha1Ctx = (SHA1_CTX*) malloc(sizeof (SHA1_CTX));

       if(sThreadData->sha1Ctx == NULL){
           printf("Failed to allocate memory for sha\n");
           //If this fails, we need to clean up the already allocated memory for the struct.
           //We check inside the clean up function to see if sha1Ctx pointer is null, so we dont try to
           //free a null pointer.
           CleanUp(sThreadData);
           return 1;
       } else{

           //Copy over filename so we can pass it to the thread.
           strncpy(sThreadData->fileName,argv[1],length);
           sThreadData->fileName[length] = '\0';

           sThreadData->bytes_in_buffer = 0;
           sThreadData->exit_program = 0;

            //Initialize count to zeroes.
           for (int i = 0; i < BYTE_RANGE; ++i) {
               sThreadData->count[i] = 0;
           }

           //initializing buffer to zero
           for (int i = 0; i < BUFFER_SIZE; ++i) {
               sThreadData->buffer[i] = 0;
           }

           iRc = pthread_mutex_init(&sThreadData->mutex, NULL);

           if(iRc != 0){
               perror("Couldn't create init mutex.\n");
               CleanUp(sThreadData);
               return 1;
           } else{
               //If mutex gets initialized, we set a flag to 1.
               //The reason is to prevent undefined behaviour in clean up function
               //if we were trying to destroy mutex and its not initialized.
               sThreadData->initialized_mutex = 1;
           }


           //Since the buffer is empty at start, we initialize sem_full to 0.
           iRc = sem_init(&sThreadData->sem_full, 0, 0);

           if(iRc != 0){
               perror("Couldn't create semaphore.\n");
               CleanUp(sThreadData);
               return 1;
           } else{
               //Same explanation as with mutex.
               sThreadData->initialized_sem_full = 1;
           }

           //Since the buffer has room at start, we initialize sem_empty to 0.
           iRc = sem_init(&sThreadData->sem_empty, 0, 1);

           if(iRc != 0){
               perror("Couldn't create semaphore.\n");
               CleanUp(sThreadData);
               return 1;
           }else{
               //Same explanation as with mutex.
               sThreadData->initialized_sem_empty = 1;
           }

           //threadA is created with the ReadFile method and passing in a void pointer to the
           //shared struct with shared memory.
           if (pthread_create(&threadA, NULL, ReadFile, (void*)sThreadData) != 0) {
               perror("Could not create thread A");
               CleanUp(sThreadData);
               return 1;
           }

           //Same as with threadA, but now creating the thread thats going to has the file.
           if (pthread_create(&threadB, NULL, CreateHash, (void*)sThreadData) != 0) {
               perror("Could not create thread B");
               CleanUp(sThreadData);
               return 1;
           }

           //Threads join before we clean up.
           if (pthread_join(threadA, NULL) != 0) {
               perror("ThreadA failed join.\n");
               CleanUp(sThreadData);
               return 1;
           }

           //Threads join before we clean up.
           if (pthread_join(threadB, NULL) != 0) {
               perror("ThreadB failed join.\n");
               CleanUp(sThreadData);
               return 1;
           }

            //Destroy mutex and semaphores
            //Freeing memory
           CleanUp(sThreadData);

           return 0;
       }//sThreadData->sha1Ctx == NULL
   } // sThreadData == NULL
}










