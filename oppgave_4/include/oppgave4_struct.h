#ifndef PG3401_V24_38_EKSAMEN_V24_OPPGAVE4_HEADERS_H
#define PG3401_V24_38_EKSAMEN_V24_OPPGAVE4_HEADERS_H

#include <pthread.h>
#include <semaphore.h>
#include "sha1.h"

#include "./oppgave4_defs.h"

typedef struct _THREAD_DATA{

    int count[BYTE_RANGE];
    unsigned char digest[20];
    SHA1_CTX *sha1Ctx;
    unsigned char buffer[BUFFER_SIZE];
    pthread_mutex_t mutex, exitMutex;
    sem_t sem_full,sem_empty;
    int bytes_in_buffer;
    int exit_program;
    int initialized_mutex;
    int initialized_sem_empty;
    int initialized_sem_full;
    char fileName[];

} THREAD_DATA;


#endif //PG3401_V24_38_EKSAMEN_V24_OPPGAVE4_HEADERS_H
