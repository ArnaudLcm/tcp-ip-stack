#ifndef __TYPES_H__
#define __TYPES_H__
#include <stdint.h>
#include <assert.h>

// IPv4 addresses are 32 bytes long: x.x.x.x with x in [0, 255]
typedef uint32_t ipv4_address_t;

// MAC addresses are 6 bytes long: 12 hexa decimals numbers
typedef uint8_t mac_address_t[6];

/*
 * Sanity checks of structs.
 */
static_assert(sizeof(mac_address_t) == 6);   // MAC addresses have to be 6 bytes
static_assert(sizeof(ipv4_address_t) == 4);  // IPv4 addressed have to be 4 bytes

#define MAC_ZERO      (mac_address_t){0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
#define MAC_BROADCAST (mac_address_t){0xff, 0xff, 0xff, 0xff, 0xff, 0xff}

#endif