#ifndef __ARP_H__
#define __ARP_H__
#include <stdbool.h>
#include <sys/types.h>
#include "ethernet.h"
#include "types.h"
#include "tcpip_socket.h"

#define ARP_REQUEST 0x0001
#define ARP_REPLY 0x0002

#define ARP_TYPE_IPV4 0x0800

#define ARP_TABLE_SIZE 30

typedef enum {
    ARP_ENTRY_FREE,
    ARP_ENTRY_BUSY,
} arp_table_entry_state;

typedef struct {
    mac_address_t hw_addr;
    ipv4_address_t ipv4_aaddr;
    int state;
    uint16_t protocol_type;
} arp_table_entry_t;

typedef arp_table_entry_t arp_table_t[ARP_TABLE_SIZE];

typedef struct __attribute__((packed)) {
    uint16_t hw_type;
    uint16_t protocol_type;
    uint8_t hw_len;
    uint8_t protocol_len;
    uint16_t operation_type;
    mac_address_t src_hwaddr;
    ipv4_address_t src_ipaddr;
    mac_address_t dst_hwaddr;
    ipv4_address_t dst_ipaddr;
} arp_packet_t;

void init_arp_table();
void handle_arp_packet(tcpip_socket* socket, ethernet_frame_t* eth_frame);
static void handle_arp_request(arp_packet_t* packet, mac_address_t local_hw, ipv4_address_t local_ipv4);
static bool update_arp_table(ipv4_address_t ip_addr, mac_address_t hw_addr, uint16_t protocol_type);
void handle_arp_packet(tcpip_socket* socket, ethernet_frame_t* eth_frame);
#endif