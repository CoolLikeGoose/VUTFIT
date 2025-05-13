using ipk_sniffer.Extenstion;
using PacketDotNet;

namespace ipk_sniffer.Packets;

public class MldPacketObject : BasePacket
{
    public MldPacketObject(MldPacket packet, IPPacket ipPacket)
    {
        Name = "MLD";
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
        return PacketType.Igmp;
    }
}