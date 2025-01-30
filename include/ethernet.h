#include "types.h"
#ifndef __ETHERNET_H__
#define __ETHERNET_H__

#define ETHERNET_TYPE_ARP 0x0806
#define ETHERNET_TYPE_IP 0x8000

// We are using TAP devices to receive Layer-2 network packets with the flag IFF_NO_PI
typedef struct __attribute__((packed)) {
    mac_address_t dst_hwaddr;
    mac_address_t src_hwaddr;
    uint16_t packet_type;
    uint8_t payload[];
} ethernet_frame_t;

#endif