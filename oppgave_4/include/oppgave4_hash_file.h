#ifndef PG3401_V24_38_EKSAMEN_V24_OPPGAVE_HASH_FILE_H
#define PG3401_V24_38_EKSAMEN_V24_OPPGAVE_HASH_FILE_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./oppgave4_headers.h"

void* Count_Bytes(void* arg) {

    THREAD_DATA *sThreadData = (THREAD_DATA*) arg;

    SHA1Init(sThreadData->sha1Ctx);

    int count = 0;

    while (1) {

        sem_wait(&sThreadData->sem_full);
        pthread_mutex_lock(&sThreadData->mutex);

        if (sThreadData->exit_program && sThreadData->bytes_in_buffer == 0) {
            pthread_mutex_unlock(&sThreadData->mutex);
            break; // Break if no bytes and reading is finished
        }

        count= 0;

        for (int i = 0; i < sThreadData->bytes_in_buffer; i++){

            count ++;
            sThreadData->count[sThreadData->buffer[i]]++;
        }

        SHA1Update(sThreadData->sha1Ctx,sThreadData->buffer,count);

        printf("Bytes: %d\n", count);

        sThreadData->bytes_in_buffer = 0;
        sem_post(&sThreadData->sem_empty);
        pthread_mutex_unlock(&sThreadData->mutex);

    }

    SHA1Final(sThreadData->digest,sThreadData->sha1Ctx);

    for (int i = 0; i < 20; ++i) {
        printf("%02x", sThreadData->digest[i]);
    }

    printf("\n");


    return NULL;

}

#endif //PG3401_V24_38_EKSAMEN_V24_OPPGAVE_HASH_FILE_H
