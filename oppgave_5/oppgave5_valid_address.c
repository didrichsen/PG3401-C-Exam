#include <arpa/inet.h>

#include "./include/oppgave5_valid_address.h"

int isValidAddress(const char *ipAddress) {
    struct sockaddr_in sa;
    if (inet_pton(AF_INET, ipAddress, &(sa.sin_addr)) != 1) {
        return 0;
    }
    return 1;
}