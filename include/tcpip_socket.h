#ifndef __TCPIP_SOCKET_H__
#define __TCPIP_SOCKET_H__
#include "ethernet.h"
#include "net_dev.h"
#include "types.h"

#define BUFF_PACKET_LEN 100
#define MAX_LEN_DEVICE_NAME 15

typedef struct {
    net_dev_t* dev;
    mac_address_t hw_addr;
    ipv4_address_t ipv4_addr;
} tcpip_socket;

tcpip_socket* new_socket(char* ip, mac_address_t hw_address, char* dev_name);
int socket_send(tcpip_socket* socket, ethernet_frame_t* eth, int packet_len);
int socket_listen(tcpip_socket* socket, char* buffer, int buffer_len);
void socket_close(tcpip_socket* socket);
#endif