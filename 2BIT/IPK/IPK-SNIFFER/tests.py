from scapy.all import *
from scapy.layers.inet6 import ICMPv6ND_NS
from scapy.contrib import *
import scapy.contrib.igmp
import argparse

def sendMld():
    print("Sending mld")
    mld_report = ICMPv6MLReport2()
    mld_report.numaddr = 1

    multicast_address = "ff02::1"
    mld_record = ICMPv6MLDMultAddrRec(rtype=4,
                                    dst=multicast_address)
    ip6 = IPv6(src="::1", dst="::1")

    packet = ip6 / mld_report / mld_record

    send(packet)

def sendIgmp():
    print("Sending igmp")
    packet = Ether() / IP(dst="127.0.0.1") / scapy.contrib.igmp.IGMP()
    sendp(packet, iface="lo")

def sendTcp():
    print("Sending TCP")
    send(IP(dst="127.0.0.1")/TCP(dport=4567, flags='S') / "GET / HTTP/1.1\r\nHost: localhost\r\n\r\n")

def sendUdp():
    print("Sending UDP")
    send(IP(dst="127.0.0.1") / UDP(dport=12345) / "Test UDP")

def sendNdp():
    print("Sending NDP")
    send(IPv6(dst="::1") / ICMPv6ND_NS(tgt="::1"))

def sendArp():
    print("Sending ARP")
    send(ARP(pdst='127.0.0.1'))

def sendICMPv6():
    print("Sending ICMPv6")
    send(IPv6(dst="::1") / ICMPv6EchoRequest())

def sendICMPv4():
    print("Sending ICMPv4")
    packet = IP(dst="127.0.0.1") / ICMP()
    send(packet)

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description="Send network packets")
    parser.add_argument("action", nargs="?", choices=["tcp", "udp", "ndp", "arp", "icmp6", "icmp4", "mld", "igmp"],
                    help="Action to perform (tcp, udp, ndp, arp, icmp6, icmp4, mld, igmp)")
    args = parser.parse_args()

    if args.action == "tcp":
        sendTcp()
    elif args.action == "udp":
        sendUdp()
    elif args.action == "ndp":
        sendNdp()
    elif args.action == "arp":
        sendArp()
    elif args.action == "icmp6":
        sendICMPv6()
    elif args.action == "icmp4":
        sendICMPv4()
    elif args.action == "mld":
        sendMld()
    elif args.action == "igmp":
        sendIgmp()
    else:
        sendTcp()
        sendUdp()
        sendNdp()
        sendArp()
        sendICMPv6()
        sendICMPv4()
        sendMld()
        sendIgmp()