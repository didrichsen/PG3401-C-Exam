#include <pthread.h>
#include <stdio.h>
#include <string.h>

#include "./include/oppgave4_struct.h"
#include "./include/sha1.h"

void* CreateHash(void* arg) {

    THREAD_DATA *sThreadData = (THREAD_DATA*) arg;

    //Doing init to initialize sha
    SHA1Init(sThreadData->sha1Ctx);

    int count = 0;

    while (1) {

        //We will wait for sem_full to increase.
        //When it increases we know that we have something to read in the buffer.
        sem_wait(&sThreadData->sem_full);
        //We lock the mutex before we enter crucial area.
        pthread_mutex_lock(&sThreadData->mutex);

        //If threadA sets exit_progran = 1 abd bytes in buffer is zero, then we leave the while loop.
        //Everything is read.
        if (sThreadData->exit_program == 1 && sThreadData->bytes_in_buffer == 0) {
            pthread_mutex_unlock(&sThreadData->mutex);
            break; // Break if no bytes and reading is finished
        }

        //Count must be set to 0 each round so pass correct value to SHA1Update.
        count= 0;

        for (int i = 0; i < sThreadData->bytes_in_buffer; i++){

            count ++;
            sThreadData->count[sThreadData->buffer[i]]++;
        }

        //Each round we call SHA1Update to update our hash.
        SHA1Update(sThreadData->sha1Ctx,sThreadData->buffer,count);

        printf("Bytes: %d\n", count);

        //Bytes in buffer are now read, so we set it to zero.
        sThreadData->bytes_in_buffer = 0;
        //We signalize to ThreadA that buffer is ready to be filled.
        sem_post(&sThreadData->sem_empty);
        //Unlock mutex, so ThreadA can enter.
        pthread_mutex_unlock(&sThreadData->mutex);

    }

    //We have now read and updated the sha, so we can create the actual has.
    SHA1Final(sThreadData->digest,sThreadData->sha1Ctx);

    //Printing out has as Hex.
    for (int i = 0; i < 20; ++i) {
        printf("%02x", sThreadData->digest[i]);
    }

    printf("\n");

    //Not returning anything from the thread.
    return NULL;
}