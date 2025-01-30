#include <arpa/inet.h>
#include <fcntl.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "ethernet.h"
#include "tcpip_socket.h"
#include "utils.h"

#define LOCAL_IP "10.0.0.4"
#define LOCAL_MAC "00:0c:29:6d:50:25"
#define TARGET_IP "10.0.0.10"
#define BUFLEN 100

int main() {
    char dev[IFNAMSIZ] = "tap3";

    mac_address_t hw_address;

    if (parse_mac_address(LOCAL_MAC, hw_address) != 0) {
        fprintf(stderr, "Failed to parse MAC address\n");
        return 1;
    }

    tcpip_socket *sock = new_socket(LOCAL_IP, hw_address, dev);

    char buffer[BUFLEN];

    while (socket_listen(sock, buffer, BUFLEN) >= 0) {
        // Do something
    }
    socket_close(sock);
    return 0;
}