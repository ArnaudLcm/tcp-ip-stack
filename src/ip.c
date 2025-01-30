#include <arpa/inet.h>
#include <stdlib.h>

#include "log.h"
#include "types.h"

ipv4_address_t parse_ipv4(char* addr) {
    uint32_t dst = 0;

    if (inet_pton(AF_INET, addr, &dst) != 1) {
        perror("Error: Parsing inet address failed");
        exit(1);
    }

    return ntohl(dst);
}
