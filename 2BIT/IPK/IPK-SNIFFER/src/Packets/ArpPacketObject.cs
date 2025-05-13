using PacketDotNet;

namespace ipk_sniffer.Packets;

public class ArpPacketObject : BasePacket
{
    public ArpPacketObject(ArpPacket packet, IPPacket ipPacket)
    {
        Name = "ARP";
        
        //IP
        SrcIp = packet.SenderProtocolAddress.ToString();
        DstIp = packet.TargetProtocolAddress.ToString();
        
        //MAC
        
        SrcMac = BitConverter.ToString(packet.SenderHardwareAddress.GetAddressBytes()).Replace("-", ":");
        DstMac = BitConverter.ToString(packet.TargetHardwareAddress.GetAddressBytes()).Replace("-", ":");
    }

    protected override string GetMacAddresses()
    {
        return $"src MAC: {SrcMac}\n" +
               $"dst MAC: {DstMac}";
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
        return PacketType.Arp;
    }
}