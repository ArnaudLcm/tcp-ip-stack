#include <stdio.h>

#include "utils.h"

void print_mac_address(const unsigned char* mac_addr) {
    printf("MAC Address: %02x:%02x:%02x:%02x:%02x:%02x\n", mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3],
           mac_addr[4], mac_addr[5]);
}

int parse_mac_address(const char* mac_str, mac_address_t mac) {
    if (sscanf(mac_str, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5]) != 6) {
        return -1;  // Parsing failed
    }
    return 0;  // Success
}
