using System.Globalization;
using System.Text;
using ipk_sniffer.Extenstion;
using PacketDotNet;
using SharpPcap;

namespace ipk_sniffer.Packets;

public abstract class BasePacket
{
    protected PacketType Type;

    protected string Name { get; set; }
    protected string Time { get; set; }
    protected string? SrcMac { get; set; }
    protected string? DstMac { get; set; }
    protected int Length { get; set; }
    protected string? SrcIp { get; set; }
    protected string? DstIp { get; set; }
    protected int? SrcPort { get; set; }
    protected int? DstPort { get; set; }
    protected byte[] Data { get; set; }

    //A slight realization of the factory pattern
    public static BasePacket? ParsePacket(RawCapture? rawPacket, IPPacket ipPacket, Packet? packet)
    {
        BasePacket? outPacket = null;
        
        if (packet is TcpPacket tcpPacket) outPacket = new TcpPacketObject(tcpPacket, ipPacket);
        else if (packet is UdpPacket udpPacket) outPacket = new UdpPacketObject(udpPacket, ipPacket);
        else if (packet is ArpPacket arpPacket) outPacket = new ArpPacketObject(arpPacket, ipPacket);
        else if (packet is NdpPacket ndpPacket) outPacket = new NdpPacketObject(ndpPacket, ipPacket);
        else if (packet is IcmpV4Packet icmpV4Packet) outPacket = new IcmpV4PacketObject(icmpV4Packet, ipPacket);
        else if (packet is IcmpV6Packet icmpV6Packet) outPacket = new IcmpV6PacketObject(icmpV6Packet, ipPacket);
        else if (packet is IgmpPacket igmpPacket) outPacket = new IgmpPacketObject(igmpPacket, ipPacket);
        else if (packet is MldPacket mldPacket) outPacket = new MldPacketObject(mldPacket, ipPacket);

        if (outPacket == null) return null;
        outPacket.Time =
            rawPacket.Timeval.Date.ToString("yyyy-MM-dd'T'HH:mm:ss.fffzzz", DateTimeFormatInfo.InvariantInfo);
        outPacket.Length = rawPacket.Data.Length;
        outPacket.Data = rawPacket.Data;

        return outPacket;
    }

    public override string ToString()
    {
        StringBuilder stringBuilder = new StringBuilder();
    
        // stringBuilder.AppendLine($"Packet type: {Name}");
        stringBuilder.AppendLine($"timestamp: {Time}");

        if (GetMacAddresses() != "")
        {
            stringBuilder.AppendLine(GetMacAddresses());
        }

        stringBuilder.AppendLine($"frame length: {Length}");
        
        if (GetIps() != "")
        {
            stringBuilder.AppendLine(GetIps());
        }
        
        if (GetPorts() != "")
        {
            stringBuilder.AppendLine(GetPorts());
        }

        stringBuilder.AppendLine("");
        stringBuilder.Append(FormatPacketData(Data));

        return stringBuilder.ToString();
    }
    
    //Write packet Data in pretty way
    private string FormatPacketData(byte[] data, int bytesPerLine = 16)
    {
        StringBuilder formattedData = new StringBuilder();

        for (int i = 0; i < data.Length; i += bytesPerLine)
        {
            formattedData.Append($"0x{i:X4}: ");

            for (int j = 0; j < bytesPerLine; j++)
            {
                if (i + j < data.Length)
                {
                    byte b = data[i + j];
                    formattedData.Append($"{b:X2} ");
                }
                else
                {
                    formattedData.Append("   ");
                }
            }

            formattedData.Append("\t");

            for (int j = 0; j < bytesPerLine; j++)
            {
                if (i + j < data.Length)
                {
                    byte b = data[i + j];
                    char c = (char)b;
                    formattedData.Append(char.IsControl(c) ? '.' : c);
                }
                else
                {
                    formattedData.Append(" ");
                }
            }

            formattedData.AppendLine();
        }

        return formattedData.ToString();
    }


    protected abstract string GetMacAddresses();
    protected abstract string GetPorts();
    protected abstract string GetIps();
    public abstract PacketType GetPacketType();
}