using System.Text;
using ipk_sniffer.Extenstion;
using PacketDotNet;
using SharpPcap;

namespace ipk_sniffer.Packets;

public class PacketParser
{
    private bool _tcp = false;
    private bool _udp = false;
    private bool _icmp4 = false;
    private bool _icmp6 = false;
    private bool _arp = false;
    private bool _ndp = false;
    private bool _igmp = false;
    private bool _mld = false;
    
    public PacketParser(ArgParse args)
    {
        _tcp = args.Tcp;
        _udp = args.Udp;
        _icmp4 = args.Icmp4;
        _icmp6 = args.Icmp6;
        _arp = args.Arp;
        _ndp = args.Ndp;
        _igmp = args.Igmp;
        _mld = args.Mld;
    }
    
    public bool ParsePacket(RawCapture rawPacket)
    {
        //Extracting packets from rawCapture
        IPPacket? ipPacket = null;
        Packet? outPacket = null;
        ExtractPacket(rawPacket, ref ipPacket, ref outPacket);
        if (ipPacket is null) return false;
        
        BasePacket? packet = BasePacket.ParsePacket(rawPacket, ipPacket, outPacket);
        
        //Filter packets and print them according to the settings
        if (packet == null) return false;
        if ((_tcp && packet.GetPacketType() == PacketType.Tcp) ||
            (_udp && packet.GetPacketType() == PacketType.Udp) ||
            (_icmp4 && packet.GetPacketType() == PacketType.Icmp4) ||
            (_icmp6 && packet.GetPacketType() == PacketType.Icmp6) ||
            (_arp && packet.GetPacketType() == PacketType.Arp) ||
            (_ndp && packet.GetPacketType() == PacketType.Ndp) ||
            (_igmp && packet.GetPacketType() == PacketType.Igmp)) //TODO: (_mld && packet is MLDv1Packet)
        {
            Console.WriteLine(packet.ToString());
            return true;
        }
        
        return false;
    }

    private void ExtractPacket(RawCapture rawCapture, ref IPPacket? ipPacket, ref Packet? outPacket)
    {
        Packet packet = Packet.ParsePacket(rawCapture.LinkLayerType, rawCapture.Data);

        //Every packet have Payload with ipv4 or ipv6 except ARP packet
        IPv4Packet ipv4 = packet.Extract<IPv4Packet>();
        IPv6Packet ipv6 = packet.Extract<IPv6Packet>();
        ArpPacket arpPacket = packet.Extract<ArpPacket>();

        if (ipv4 != null)
        {
            ipPacket = ipv4;
        } else if (ipv6 != null)
        {
            ipPacket = ipv6;
        } else if (arpPacket != null)
        {
            ipPacket = new IPv6Packet(arpPacket.SenderProtocolAddress,
                arpPacket.TargetProtocolAddress);
            outPacket = arpPacket;
            return;
        }

        if ((outPacket = packet.Extract<TcpPacket>()) != null)
        {
            return;
        }
        if ((outPacket = packet.Extract<UdpPacket>()) != null)
        {
            return;
        }
        if ((outPacket = packet.Extract<NdpPacket>()) != null)
        {
            return;
        }

        //Mld packet is a type of IcmpV6 so we need to detect this 
        IcmpV6Packet icmpV6Packet = packet.Extract<IcmpV6Packet>();
        if (icmpV6Packet != null)
        {
            if (icmpV6Packet.Type == IcmpV6Type.MulticastListenerReport ||
                icmpV6Packet.Type == IcmpV6Type.Version2MulticastListenerReport)
            {
                outPacket = new MldPacket();
                return;
            }

            outPacket = icmpV6Packet;
            return;
        }
        if ((outPacket = packet.Extract<IcmpV4Packet>()) != null)
        {
            return;
        }

        if ((outPacket = packet.Extract<IgmpPacket>()) != null ||
            (outPacket = packet.Extract<IgmpV2Packet>()) != null)
        {
            return;
        }
    }
}