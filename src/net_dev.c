#include <arpa/inet.h>
#include <fcntl.h>
#include <linux/if.h>
#include <linux/if_arp.h>
#include <linux/if_tun.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "net_dev.h"
#include "types.h"

int set_if_ip_and_netmask(net_dev_t* dev, const char* ip_addr, const char* netmask) {
    struct ifreq ifr;
    struct sockaddr_in addr;

    memset(&ifr, 0, sizeof(ifr));

    // Set the interface name
    strncpy(ifr.ifr_name, dev->if_name, IFNAMSIZ);

    // Set IP address
    addr.sin_family = AF_INET;
    if (inet_pton(AF_INET, ip_addr, &addr.sin_addr) <= 0) {
        perror("Error: Invalid IP address format");
        return -1;
    }
    memcpy(&ifr.ifr_addr, &addr, sizeof(struct sockaddr_in));

    // Set IP address using ioctl
    if (ioctl(dev->ctrl_fd, SIOCSIFADDR, &ifr) == -1) {
        perror("Error: Can't set IP address");
        return -1;
    }

    // Set Netmask
    if (inet_pton(AF_INET, netmask, &addr.sin_addr) <= 0) {
        perror("Error: Invalid netmask format");
        return -1;
    }
    memcpy(&ifr.ifr_addr, &addr, sizeof(struct sockaddr_in));

    // Set netmask using ioctl
    if (ioctl(dev->ctrl_fd, SIOCSIFNETMASK, &ifr) == -1) {
        perror("Error: Can't set netmask");
        return -1;
    }

    return 0;
}

int set_if_hw(net_dev_t* dev, mac_address_t hw_addr) {
    struct ifreq ifr;

    memset(&ifr, '\0', sizeof ifr);
    memcpy(ifr.ifr_name, dev->if_name, sizeof dev->if_name);

    ifr.ifr_hwaddr.sa_family = ARPHRD_ETHER;
    memcpy(ifr.ifr_hwaddr.sa_data, hw_addr, 6);

    if (ioctl(dev->ctrl_fd, SIOCSIFHWADDR, &ifr) == -1) {
        perror("Error: Can't set link-layer address");
        return -1;
    }
    return 0;
}

int set_if_up(net_dev_t* dev) {
    struct ifreq ifr;
    int sockfd;

    // Open a socket to perform the ioctl 
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Error opening socket to set interface up");
        return -1;
    }

    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, dev->if_name, IFNAMSIZ);

    /* Set the interface up */
    ifr.ifr_flags |= IFF_UP;
    if (ioctl(sockfd, SIOCSIFFLAGS, &ifr) < 0) {
        perror("Error setting interface up");
        close(sockfd);
        return -1;
    }
    close(sockfd);
    return 0;
}
