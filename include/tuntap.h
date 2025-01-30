#ifndef __TUNTAP_H__
#define __TUNTAP_H__
#include "tcpip_socket.h"
#include "net_dev.h"
#define CLEAR(x) memset(&(x), 0, sizeof(x))

int tap_alloc(net_dev_t* dev, char* dev_name, mac_address_t hw_addr, char* ipv4);
void tuntap_destroy(int fd);
int tap_read(int fd, char* buffer, int buff_len);
int tap_write(int fd, char* content, int content_len);
#endif