sudo hping3 -2 -c 1 -p 0 192.168.1.1 #arp
ping -c 4 192.168.1.1 #icmpv4
ping6 -c 4 ::1 # ICMPv6 

nc -l localhost 3333
nc localhost 3333 #tcp

nc -u -l localhost 3333
nc -u localhost 3333 #tcp