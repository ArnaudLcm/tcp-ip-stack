#include <arpa/inet.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "arp.h"
#include "log.h"
#include "tcpip_socket.h"

static arp_table_t arp_table;

void init_arp_table() {
    arp_table_entry_t* entry;
    for (size_t i = 0; i < ARP_TABLE_SIZE; i++) {
        entry = &arp_table[i];
        entry->state = ARP_ENTRY_FREE;
    }
}

static void handle_arp_request(arp_packet_t* packet, mac_address_t local_hw, ipv4_address_t local_ipv4) {
    // Prepare ARP reply by swapping source and destination fields
    packet->dst_ipaddr = packet->src_ipaddr;
    memcpy(packet->dst_hwaddr, packet->src_hwaddr, sizeof(packet->dst_hwaddr));

    packet->src_ipaddr = htonl(local_ipv4);  // Ensure local IP is in network byte order
    memcpy(packet->src_hwaddr, local_hw, sizeof(packet->src_hwaddr));

    // Set the operation type to ARP reply
    packet->operation_type = ARP_REPLY;
    packet->src_ipaddr = htonl(local_ipv4);
    packet->dst_ipaddr = htonl(packet->dst_ipaddr);
}

static bool add_arp_table(ipv4_address_t ip_addr, mac_address_t hw_addr, uint16_t protocol_type) {
    arp_table_entry_t* entry;
    for (size_t i = 0; i < ARP_TABLE_SIZE; i++) {
        entry = &arp_table[i];
        if (entry->state == ARP_ENTRY_FREE) {
            memcpy(entry->hw_addr, hw_addr, sizeof(entry->hw_addr));
            entry->ipv4_aaddr = ip_addr;
            entry->protocol_type = protocol_type;
            entry->state = ARP_ENTRY_BUSY;
            return true;
        }
    }
    print_err("Error: No more entry available in the arp table.");
    return false;
}

static bool update_arp_table(ipv4_address_t ip_addr, mac_address_t hw_addr, uint16_t protocol_type) {
    arp_table_entry_t* entry;
    for (size_t i = 0; i < ARP_TABLE_SIZE; i++) {
        entry = &arp_table[i];
        if (entry->state == ARP_ENTRY_BUSY && entry->protocol_type == protocol_type && entry->ipv4_aaddr == ip_addr) {
            memcpy(entry->hw_addr, hw_addr, sizeof(entry->hw_addr));
            return true;
        }
    }

    return false;
}

void handle_arp_packet(tcpip_socket* socket, ethernet_frame_t* eth_frame) {
    arp_packet_t* arp_packet = (arp_packet_t*)eth_frame->payload;
    arp_packet->operation_type = ntohs(arp_packet->operation_type);
    arp_packet->protocol_type = ntohs(arp_packet->protocol_type);

    if (arp_packet == NULL) {
        return;
    }

    // Ensure we only handle IPv4 ARP packets
    if (arp_packet->protocol_type != ARP_TYPE_IPV4) {
        print_err("Error: Only support ARP IPV4 packets");
        return;
    }

    // Update or add ARP table entry for the sender
    if (!update_arp_table(ntohl(arp_packet->src_ipaddr), arp_packet->src_hwaddr, arp_packet->operation_type)) {
        add_arp_table(ntohl(arp_packet->src_ipaddr), arp_packet->src_hwaddr, arp_packet->protocol_type);
    }

    switch (arp_packet->operation_type) {
        case ARP_REQUEST:
            handle_arp_request(arp_packet, socket->hw_addr, socket->ipv4_addr);
            break;

        default:
            print_err("Error: Only support ARP Requests.");
            return;
    }

    int len = sizeof(arp_packet_t);
    arp_packet->operation_type = htons(arp_packet->operation_type);
    arp_packet->hw_type = htons(arp_packet->hw_type);
    arp_packet->protocol_type = htons(arp_packet->protocol_type);

    eth_frame->packet_type = htons(ETHERNET_TYPE_ARP);
    memcpy(eth_frame->src_hwaddr, socket->hw_addr, sizeof(eth_frame->src_hwaddr));
    memcpy(eth_frame->dst_hwaddr, arp_packet->dst_hwaddr, sizeof(eth_frame->dst_hwaddr));

    len += sizeof(ethernet_frame_t);

    if (socket_send(socket, eth_frame, len) < 0) {
        perror("Error: An error occurred while sending an ARP packet.");
    }
}
