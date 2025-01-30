#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "arp.h"
#include "ethernet.h"
#include "ip.h"
#include "net_dev.h"
#include "tcpip_socket.h"
#include "tuntap.h"
#include "types.h"

tcpip_socket* new_socket(char* ip, mac_address_t hw_address, char* dev_name) {
    tcpip_socket* socket = malloc(sizeof(tcpip_socket));
    net_dev_t* net_dev = malloc(sizeof(net_dev_t));
    if (!socket) {
        perror("Failed to allocate tcpip_socket");
        exit(1);
    }

    socket->ipv4_addr = parse_ipv4(ip);
    memcpy(socket->hw_addr, hw_address, sizeof(socket->hw_addr));

    if (tap_alloc(net_dev, dev_name, hw_address, ip) == false) {
        perror("Failed to allocate tap device");
        free(socket);
        exit(1);
    }

    socket->dev = net_dev;

    init_arp_table();

    return socket;
}

int socket_listen(tcpip_socket* socket, char* buffer, int buffer_len) {
    char packet_buffer[BUFF_PACKET_LEN];

    int nread = tap_read(socket->dev->packet_fd, packet_buffer, BUFF_PACKET_LEN);
    if (nread > 0) {
        ethernet_frame_t* eth_frame = (ethernet_frame_t*)packet_buffer;
        eth_frame->packet_type = ntohs(eth_frame->packet_type);
        switch (eth_frame->packet_type) {
            case (ETHERNET_TYPE_ARP):
                handle_arp_packet(socket, eth_frame);
                break;
            default:
                break;
        }
    }

    return nread;
}

int socket_send(tcpip_socket* socket, ethernet_frame_t* eth, int packet_len) {
    return tap_write(socket->dev->packet_fd, (char*)eth, packet_len);
}

void socket_close(tcpip_socket* socket) {
    close(socket->dev->ctrl_fd);
    close(socket->dev->packet_fd);
    free(socket->dev);
    free(socket);
}