#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./include/oppgave4_struct.h"
#include "./include/oppgave4_read_file.h"

void* ReadFile(void* arg) {

    THREAD_DATA *sThreadData = (THREAD_DATA*) arg;

    int count = 0;

    FILE* fp = fopen(sThreadData->fileName, "rb");
    if (fp == NULL) {
        printf("File couldn't be opened. Signalizing to other threads to exit.\n");
        //We set exit_program and open up for threadB to continue.
        //It will exit since exit_program = 1 and bytes in buffer = 0;
        sThreadData->exit_program = 1;
        sThreadData->bytes_in_buffer = 0;
        sem_post(&sThreadData->sem_full);
        pthread_mutex_unlock(&sThreadData->mutex);
    } else{
        while (1) {

            //We check to see if the buffer has room.
            //It is set to 1 initially, so we can enter here the first time we enter the thread.
            sem_wait(&sThreadData->sem_empty);
            //Lock mutex to operate in critical area.
            pthread_mutex_lock(&sThreadData->mutex);

            //reading bytes and making sure we dont read more than BUFFER_SIZE
            int read_bytes = fread(sThreadData->buffer + sThreadData->bytes_in_buffer, 1, BUFFER_SIZE - sThreadData->bytes_in_buffer, fp);
            sThreadData->bytes_in_buffer += read_bytes;

            //Signalize to ThreadB that we are finished.
            if (read_bytes == 0 && feof(fp)) {
                sThreadData->exit_program = 1;
                sem_post(&sThreadData->sem_full);
                pthread_mutex_unlock(&sThreadData->mutex);
                break;
            }

            //Signalizing to other threads that there is something in the buffer.
            sem_post(&sThreadData->sem_full);
            //Unlock mutex so other threads can read from buffer.
            pthread_mutex_unlock(&sThreadData->mutex);

        }
    }

    if(fp != NULL){
        fclose(fp);
    }
    //Nothing to return from thread.
    return NULL;
}