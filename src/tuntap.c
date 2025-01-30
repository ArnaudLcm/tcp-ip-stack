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
#include "tuntap.h"
#include "types.h"

int tap_alloc(net_dev_t* dev, char* dev_name, mac_address_t hw_addr, char* ipv4) {
    struct ifreq ifr;
    int fd;

    // Open the TAP device file
    if ((fd = open("/dev/net/tun", O_RDWR)) < 0) {
        perror("Cannot open /dev/net/tun");
        return false;
    }

    // Clear ifr structure
    memset(&ifr, 0, sizeof(ifr));

    ifr.ifr_flags = IFF_TAP | IFF_NO_PI;  // TAP device, no packet information

    // Set the interface name, if provided
    if (*dev_name) {
        strncpy(ifr.ifr_name, dev_name, IFNAMSIZ);
    }

    // Create the device
    if (ioctl(fd, TUNSETIFF, (void*)&ifr) < 0) {
        perror("Cannot create TAP interface");
        close(fd);
        return false;
    }

    // Copy the assigned device name back to dev

    // Now, open a socket for further ioctl calls (for getting flags, etc.)
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Cannot create socket");
        close(fd);
        return false;
    }

    // Assign the socket descriptor to dev->ctrl_fd 
    dev->ctrl_fd = sockfd;
    dev->packet_fd = fd;

    strcpy(dev_name, ifr.ifr_name);

    if (ioctl(dev->ctrl_fd, SIOCGIFFLAGS, &ifr) == -1) {
        perror("Error: Can't get interface values");
        return false;
    }

    strcpy(dev->if_name, ifr.ifr_name);
    if (set_if_up(dev) < 0) {
        close(fd);
        return false;
    }

    // Set the hardware address (MAC address)
    if (set_if_hw(dev, hw_addr) < 0) {
        close(fd);
        return false;
    }

    // Set the IPv4 address
    if (set_if_ip_and_netmask(dev, ipv4, "255.255.255.0") < 0) {
        close(fd);
        return false;
    }

    return true;
}

void tuntap_destroy(int fd) {
    if (ioctl(fd, TUNSETPERSIST, 0) == -1) {
        perror("Error: Can't destroy the interface");
    }
}

int tap_write(int fd, char* content, int content_len) { return write(fd, content, content_len); }

int tap_read(int fd, char* buffer, int buff_len) { return read(fd, buffer, buff_len); }