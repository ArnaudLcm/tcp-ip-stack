# utils.gdb (Only Python Code)
import gdb

# Function to print MAC address in a human-readable format
def print_mac_address(mac_addr):
    """Print MAC address in readable format"""
    print(f"{mac_addr[0]:02x}:{mac_addr[1]:02x}:{mac_addr[2]:02x}:{mac_addr[3]:02x}:{mac_addr[4]:02x}:{mac_addr[5]:02x}")

# Function to print IPv4 address in a human-readable format
def print_ipv4_address(ip_addr):
    """Print IPv4 address in readable format"""
    ip = (ip_addr >> 24) & 0xFF, (ip_addr >> 16) & 0xFF, (ip_addr >> 8) & 0xFF, ip_addr & 0xFF
    print(f"{ip[0]}.{ip[1]}.{ip[2]}.{ip[3]}")

# Function to print packet type
def print_packet_type(packet_type):
    """Print the packet type"""
    print(f"Packet Type: {packet_type}")

# Registering custom GDB commands
class PrintMac(gdb.Command):
    """Command to print MAC address in readable format"""
    def __init__(self):
        super(PrintMac, self).__init__("print_mac", gdb.COMMAND_USER)

    def invoke(self, arg, from_tty):
        # Parse the argument (assumed to be a variable in GDB)
        mac_addr = gdb.parse_and_eval(arg)
        print_mac_address(mac_addr)

class PrintIP(gdb.Command):
    """Command to print IPv4 address in readable format"""
    def __init__(self):
        super(PrintIP, self).__init__("print_ip", gdb.COMMAND_USER)

    def invoke(self, arg, from_tty):
        # Parse the argument (assumed to be a variable in GDB)
        ip_addr = gdb.parse_and_eval(arg)
        print_ipv4_address(ip_addr)

class PrintPacketType(gdb.Command):
    """Command to print the packet type"""
    def __init__(self):
        super(PrintPacketType, self).__init__("print_pkt_type", gdb.COMMAND_USER)

    def invoke(self, arg, from_tty):
        # Parse the argument (assumed to be a variable in GDB)
        packet_type = gdb.parse_and_eval(arg)
        print_packet_type(packet_type)

# Instantiate and register the commands
PrintMac()
PrintIP()
PrintPacketType()
