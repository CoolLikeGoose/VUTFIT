using PacketDotNet;


namespace ipk_sniffer.Packets;

public class NdpPacketObject : BasePacket
{
    public NdpPacketObject(NdpPacket packet, IPPacket ipPacket)
    {
        Name = "NDP";
        //IP
        // IPv6Packet ipPacket = (IPv6Packet)packet.ParentPacket;
        SrcIp = ipPacket.SourceAddress.ToString();
        DstIp = ipPacket.DestinationAddress.ToString();
        
        //MAC 
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
        return PacketType.Ndp;
    }
}