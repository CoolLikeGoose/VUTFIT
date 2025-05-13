using PacketDotNet;

namespace ipk_sniffer.Packets;

public class IgmpPacketObject : BasePacket
{
    public IgmpPacketObject(IgmpPacket packet, IPPacket ipPacket)
    {
        Name = "Igmp";
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
        return $"src port: {SrcIp}\n" +
               $"dst port: {DstIp}";
    }

    public override PacketType GetPacketType()
    {
        return PacketType.Igmp;
    }
}