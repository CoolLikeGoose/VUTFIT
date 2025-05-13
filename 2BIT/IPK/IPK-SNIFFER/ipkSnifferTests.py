from scapy.all import *
from scapy.layers.inet6 import ICMPv6ND_NS
from scapy.contrib import *
import unittest
import subprocess
import os
from termcolor import colored, cprint
from time import sleep
import signal

debug = False

src_ipv6 = "::1"
dst_ipv6 = "::1"
binary_path = "./ipk-sniffer"
sleep_time = 2

def sendMldReport():
    mld_report = ICMPv6MLReport2()
    mld_report.numaddr = 1

    multicast_address = "ff02::1"
    mld_record = ICMPv6MLDMultAddrRec(rtype=4,
                                    dst=multicast_address)
    ip6 = IPv6(src=src_ipv6, dst=dst_ipv6)

    packet = ip6 / mld_report / mld_record

    send(packet)

def sendNdpPacket():
    ns = ICMPv6ND_NS(tgt=dst_ipv6)

    ll_addr = "00:01:02:03:04:05"
    src_ll_opt = ICMPv6NDOptSrcLLAddr(lladdr=ll_addr)
    ip6 = IPv6(src=src_ipv6, dst=dst_ipv6)

    packet = ip6 / ns / src_ll_opt

    send(packet)

# Funkce pro odeslání ICMPv6 Echo Request (ping)
def sendIcmpv6Echo_message():
    echo_req = ICMPv6EchoRequest(data="Hello IPv6!")
    ip6 = IPv6(src=src_ipv6, dst=dst_ipv6)

    packet = ip6 / echo_req

    send(packet)

# Funkce pro odeslání Router Solicitation zprávy NDP
def sendRouterSolicitation():
    rs = ICMPv6ND_RS()
    ip6 = IPv6(src=src_ipv6, dst=src_ipv6)  

    packet = ip6 / rs

    send(packet)

def sendIcmpv4():
    packet = IP(dst="127.0.0.1") / ICMP()
    send(packet)

# Funkce pro odeslání ICMPv6 Echo Request
def sendIcmpv6Echo():
    packet = IPv6(dst="::1") / ICMPv6EchoRequest()
    send(packet)

# Funkce pro odeslání ARP Request
def sendArpRequest():
    packet = ARP(pdst='127.0.0.1')
    send(packet)

# Funkce pro odeslání NDP Neighbor Solicitation
def sendNdpNs():
    packet = IPv6(dst="::1") / ICMPv6ND_NS(tgt="::1")
    send(packet)

# Funkce pro odeslání IGMP paketu
def sendIgmp():
    packet = IP(dst="127.0.0.1")/IGMP(type="0x11", mrcode=200)
    send(packet)

# Funkce pro odeslání UDP paketu
def sendUdpPacket():
    packet = IP(dst="127.0.0.1") / UDP(dport=12345) / "Test UDP"
    send(packet)

# Funkce pro odeslání TCP paketu
def sendTcpPacket():
    packet = IP(dst="127.0.0.1")/TCP(dport=80, flags='S') / "GET / HTTP/1.1\r\nHost: localhost\r\n\r\n"
    send(IP(dst="127.0.0.1")/TCP(dport=4567, flags='S') / "GET / HTTP/1.1\r\nHost: localhost\r\n\r\n")


class SnifferTests(unittest.TestCase):
    binary_args = ["-i", "lo", "--tcp"]  # předpokládám argumenty pro sniffer

    def read_stdout(self, queue):
        for line in iter(self.sniffer.stdout.readline, ""):
            if debug:
                print(colored("STDOUT:", "blue"), colored(line, "blue"), end="")
            queue.put(line)

    def read_stderr(self, queue):
        for line in iter(self.sniffer.stderr.readline, ""):
            if debug:
                print(colored("STDERR:", "magenta"), colored(line, "magenta"), end="")
            queue.put(line)

    def execute(self, input_data):
        self.sniffer.stdin.write(input_data + "\n")
        self.sniffer.stdin.flush()

        sleep(0.2)

    def get_stdout(self):
        sleep(sleep_time)
        output = []
        while not self.stdout_queue.empty():
            output.append(self.stdout_queue.get())
        return "".join(output)

    def get_stderr(self):
        output = []
        while not self.stderr_queue.empty():
            output.append(self.stderr_queue.get())
        return "".join(output)
    
    def startSniffer(self, args):
        sleep(0.2)
        self.stdout_queue = queue.Queue()
        self.stderr_queue = queue.Queue()
        try:
            self.sniffer = subprocess.Popen([binary_path] + args, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True, bufsize=1)
        except Exception as e:
            self.fail(f"Error executing binary file: {e}")

        self._start_thread(self.read_stdout, self.stdout_queue)
        self._start_thread(self.read_stderr, self.stderr_queue)
        self.return_code = None

        sleep(1)  # Give some time for the process to start

    def _start_thread(self, target, queue):
        thread = threading.Thread(target=target, args=(queue,))
        thread.daemon = True  # Thread dies with the program
        thread.start()


    def tearDown(self):
        self.sniffer.send_signal(signal.SIGINT)
        self.sniffer.terminate() 
        try:
            self.sniffer.wait(timeout=2)  
        except subprocess.TimeoutExpired:
            self.sniffer.kill()  
        self.sniffer.stdout.close()
        self.sniffer.stderr.close()

    def testTcp(self):
        args = ["-i", "any", "--tcp", "-p", "4567"]
        self.startSniffer(args)
        # Zachycení a kontrola výstupu snifferu
        sendTcpPacket()

        output = self.get_stdout()
        self.assertIn("src IP: 127.0.0.1", output, "IP SRC not found in appropriate format")
        self.assertIn("dst port: 4567", output, "Frame length not found in appropriate format")

    def testUdp(self):
        args = ["-i", "any", "--udp", "-p", "12345"]
        self.startSniffer(args)
        # Zachycení a kontrola výstupu snifferu
        sendUdpPacket()

        output = self.get_stdout()
        self.assertIn("src IP: 127.0.0.1", output, "IP SRC not found in appropriate format")
        self.assertIn("dst port: 12345", output, "Frame length not found in appropriate format")

    def testNdp(self):
        args = ["-i", "any", "--ndp"]
        self.startSniffer(args)
        # Zachycení a kontrola výstupu snifferu
        sendNdpNs()

        output = self.get_stdout()
        self.assertIn("src IP: ::1", output, "IP SRC not found in appropriate format")
        self.assertIn("frame length: 78", output, "Frame length not found in appropriate format")

    def testNdp2(self):
        args = ["-i", "any", "--ndp"]
        self.startSniffer(args)
        # Zachycení a kontrola výstupu snifferu
        sendRouterSolicitation()

        output = self.get_stdout()
        self.assertIn("src IP: ::1", output, "IP SRC not found in appropriate format")
        self.assertIn("frame length: 62", output, "Frame length not found in appropriate format")

    def testArp(self):
        args = ["-i", "any", "--arp"]
        self.startSniffer(args)
        # Zachycení a kontrola výstupu snifferu
        sendArpRequest()

        output = self.get_stdout()
        self.assertIn("src MAC: 00:00:00:00:00:00", output, "MAC SRC not found in appropriate format")
        self.assertIn("frame length: 42", output, "Frame length not found in appropriate format")

    def testIcmpv6(self):
        args = ["-i", "any", "--icmp6"]
        self.startSniffer(args)
        # Zachycení a kontrola výstupu snifferu
        sendIcmpv6Echo()

        output = self.get_stdout()
        self.assertIn("src IP: ::1", output, "IP SRC not found in appropriate format")
        self.assertIn("frame length: 62", output, "Frame length not found in appropriate format")

    def testIcmpv62(self):
        args = ["-i", "any", "--icmp6"]
        self.startSniffer(args)
        # Zachycení a kontrola výstupu snifferu
        sendIcmpv6Echo_message()

        output = self.get_stdout()
        self.assertIn("src IP: ::1", output, "IP SRC not found in appropriate format")
        self.assertIn("frame length: 73", output, "Frame length not found in appropriate format")


    def testIcmpv4(self):
        args = ["-i", "any", "--icmp4"]
        self.startSniffer(args)
        # Zachycení a kontrola výstupu snifferu
        sendIcmpv4()

        output = self.get_stdout()
        self.assertIn("src IP: 127.0.0.1", output, "IP SRC not found in appropriate format")
        self.assertIn("frame length: 42", output, "Frame length not found in appropriate format")

if __name__ == '__main__':
    unittest.main()
