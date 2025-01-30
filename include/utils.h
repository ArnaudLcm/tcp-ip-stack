#ifndef __UTILS_H__
#define __UTILS_H__
#include "types.h"
void print_mac_address(const unsigned char* mac_addr);
int parse_mac_address(const char *mac_str, mac_address_t mac);
#endif