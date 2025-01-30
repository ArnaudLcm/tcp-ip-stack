#ifndef __NET_DEV_H__
#define __NET_DEV_H__
#include <linux/if.h>
#include "types.h"

typedef struct {
    int packet_fd;
    int ctrl_fd;
    mac_address_t	hw_addr;
	char		if_name[IFNAMSIZ + 1];
} net_dev_t;

int set_if_ip_and_netmask(net_dev_t* dev, const char* ip_addr, const char* netmask);
int set_if_up(net_dev_t* dev);
int set_if_hw(net_dev_t* dev, mac_address_t hw_addr);

#endif