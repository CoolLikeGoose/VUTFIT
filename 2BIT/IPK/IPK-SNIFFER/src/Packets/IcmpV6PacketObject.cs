using PacketDotNet;

namespace ipk_sniffer.Packets;

public class IcmpV6PacketObject : BasePacket
{
    public IcmpV6PacketObject(IcmpV6Packet packet, IPPacket ipPacket)
    {
        Name = "IcmpV6";
        //IP
        SrcIp = ipPacket.SourceAddress.ToString();
        DstIp = ipPacket.DestinationAddress.ToString();
    }

    protected override string GetMacAddresses()
    {
        return "";
    }

    protected override string GetPorts()
    {
        return "";
    }

    protected override string GetIps()
    {
        return $"src IP: {SrcIp}\n" +
               $"dst IP: {DstIp}";
    }

    public override PacketType GetPacketType()
    {
        return PacketType.Icmp6;
    }
}