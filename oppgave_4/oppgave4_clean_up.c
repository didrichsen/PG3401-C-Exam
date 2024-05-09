#include <stdlib.h>

#include "./include/oppgave4_clean_up.h"

//We want to clean up at different places in application
//but some places mutexes, sem and sha might not be initialized,
//so we need to check this before destroying and freeing.

void CleanUp(THREAD_DATA *sThreadData) {
    if (sThreadData != NULL) {
        if (sThreadData->initialized_mutex == 1)
            pthread_mutex_destroy(&sThreadData->mutex);
        if (sThreadData->initialized_sem_empty == 1)
            sem_destroy(&sThreadData->sem_empty);
        if (sThreadData->initialized_sem_full == 1)
            sem_destroy(&sThreadData->sem_full);
        if(sThreadData->sha1Ctx != NULL){
            free(sThreadData->sha1Ctx);
        }
        free(sThreadData);
    }
}