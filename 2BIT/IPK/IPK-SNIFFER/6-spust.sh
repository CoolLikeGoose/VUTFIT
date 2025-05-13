#!/bin/sh
IFACE="-i lo"
RESULTFILE=".testresult"
TCPPORTBASE=65000
UDPPORTBASE=65100
IPV4PORTOFFSET=4
IPV6PORTOFFSET=6

#Print any readme
echo -e "\033[0;34mREADME\033[0m" 
cat $1/R*.md $1/r*.md
echo -e "\033[0;34mCHANGELOG\033[0m" 
cat $1/CHANG*.md


echo "Press Enter to continue..."
read any

#Mask Ctrl+C not to interrupt script but only single execution
trap ' ' INT

echo -e "\033[0;34mT0: Vypis interfaceu\033[0m" | tee -a $1/$RESULTFILE
#echo $1/ipk-sniffer | tee -a $1/$RESULTFILE
$1/ipk-sniffer | tee -a $1/$RESULTFILE
$1/ipk-sniffer -i | tee -a $1/$RESULTFILE

echo -e "\033[0;34mT1: IPv4 a TCP (74,74,66,99)\033[0m" | tee -a $1/$RESULTFILE
echo $1/ipk-sniffer $IFACE -p $(($TCPPORTBASE+$IPV4PORTOFFSET)) --tcp -n 4 | tee -a $1/$RESULTFILE
$1/ipk-sniffer $IFACE -p $(($TCPPORTBASE+$IPV4PORTOFFSET)) --tcp -n 4 | tee -a $1/$RESULTFILE

echo -e "\033[0;34mT2: IPv6 a TCP (94,94,86,119)\033[0m" | tee -a $1/$RESULTFILE
echo $1/ipk-sniffer $IFACE -p $(($TCPPORTBASE+$IPV6PORTOFFSET)) --tcp -n 4 | tee -a $1/$RESULTFILE
$1/ipk-sniffer $IFACE -p $(($TCPPORTBASE+$IPV6PORTOFFSET)) --tcp -n 4 | tee -a $1/$RESULTFILE

echo -e "\033[0;34mT3: IPv4 a UDP (75)\033[0m"  | tee -a $1/$RESULTFILE
echo $1/ipk-sniffer $IFACE -p $(($UDPPORTBASE+$IPV4PORTOFFSET)) --udp -n 1 | tee -a $1/$RESULTFILE
$1/ipk-sniffer $IFACE -p $(($UDPPORTBASE+$IPV4PORTOFFSET)) --udp -n 1 | tee -a $1/$RESULTFILE

echo -e "\033[0;34mT4: IPv6 a UDP (95)\033[0m" | tee -a $1/$RESULTFILE
echo $1/ipk-sniffer $IFACE -p $(($UDPPORTBASE+$IPV6PORTOFFSET)) --udp -n 1 | tee -a $1/$RESULTFILE
$1/ipk-sniffer $IFACE -p $(($UDPPORTBASE+$IPV6PORTOFFSET)) --udp -n 1 | tee -a $1/$RESULTFILE

echo -e "\033[0;34mT5: IPv4 a ICMP (98,98,98,98)\033[0m" | tee -a $1/$RESULTFILE
echo $1/ipk-sniffer $IFACE --icmp4 -n 4 | tee -a $1/$RESULTFILE
$1/ipk-sniffer $IFACE --icmp4 -n 4 | tee -a $1/$RESULTFILE

echo -e "\033[0;34mT6: IPv6 a ICMP (118,118,118,118)\033[0m" | tee -a $1/$RESULTFILE
echo $1/ipk-sniffer $IFACE --icmp6 -n 4 | tee -a $1/$RESULTFILE
$1/ipk-sniffer $IFACE --icmp6 -n 4 | tee -a $1/$RESULTFILE

echo -e "\033[0;34mT7: ARP (60,42)\033[0m" | tee -a $1/$RESULTFILE
echo $1/ipk-sniffer $IFACE --arp -n 4 | tee -a $1/$RESULTFILE
$1/ipk-sniffer $IFACE --arp -n 4 | tee -a $1/$RESULTFILE

echo -e "\033[0;34mT8: NDP (,)\033[0m" | tee -a $1/$RESULTFILE
echo $1/ipk-sniffer $IFACE --ndp -n 2 | tee -a $1/$RESULTFILE
$1/ipk-sniffer $IFACE --ndp -n 2 | tee -a $1/$RESULTFILE

echo -e "\033[0;34mT10: IGMP (,)\033[0m" | tee -a $1/$RESULTFILE
echo $1/ipk-sniffer $IFACE --igmp -n 4 | tee -a $1/$RESULTFILE
$1/ipk-sniffer $IFACE --igmp -n 4 | tee -a $1/$RESULTFILE

echo -e "\033[0;34mT11: MLD (,)\033[0m" | tee -a $1/$RESULTFILE
echo $1/ipk-sniffer $IFACE --mld -n 2 | tee -a $1/$RESULTFILE
$1/ipk-sniffer $IFACE --mld -n 2 | tee -a $1/$RESULTFILE

#echo -e "\033[0;34mT8: MLD (,)\033[0m" | tee -a $1/$RESULTFILE
#$1/ipk-sniffer $IFACE --mld -n 4 | tee -a $1/$RESULTFILE

echo -e "\033[0;34mT9: Filtry a ukonceni\033[0m" | tee -a $1/$RESULTFILE
echo $1/ipk-sniffer $IFACE --tcp --udp -p $(($TCPPORTBASE+$IPV4PORTOFFSET)) --icmp4 --arp -n 17 | tee -a $1/$RESULTFILE
$1/ipk-sniffer $IFACE --tcp --udp -p $(($TCPPORTBASE+$IPV4PORTOFFSET)) --icmp4 --arp -n 17 | tee -a $1/$RESULTFILE

