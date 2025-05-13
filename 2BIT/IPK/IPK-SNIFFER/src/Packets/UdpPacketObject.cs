using PacketDotNet;

namespace ipk_sniffer.Packets;

public class UdpPacketObject : BasePacket
{
    public UdpPacketObject(UdpPacket packet, IPPacket ipPacket)
    {
        Name = "UDP";
        //IP
        SrcIp = ipPacket.SourceAddress.ToString();
        DstIp = ipPacket.DestinationAddress.ToString();
        //Port
        SrcPort = packet.SourcePort;
        DstPort = packet.DestinationPort;
    }

    protected override string GetMacAddresses()
    {
        return "";
    }

    protected override string GetPorts()
    {
        return $"src port: {SrcPort}\n" +
               $"dst port: {DstPort}";
    }

    protected override string GetIps()
    {
        return $"src IP: {SrcIp}\n" +
               $"dst IP: {DstIp}";
    }

    public override PacketType GetPacketType()
    {
        return PacketType.Udp;
    }
}