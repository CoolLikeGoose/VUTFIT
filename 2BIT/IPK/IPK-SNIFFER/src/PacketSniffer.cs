using ipk_sniffer.Packets;
using PacketDotNet;
using SharpPcap;

namespace ipk_sniffer;

public class PacketSniffer
{
    private readonly bool _showInterface;
    private readonly string? _sniffInterface;
    private readonly string? _portFilter;
    
    private readonly int _packetMaxNumber;
    private int _packetsGot = 0;

    private readonly PacketParser _parser;
    public PacketSniffer(ArgParse args)
    {
        _parser = new PacketParser(args);
        
        _packetMaxNumber = args.NumberOfPackets;
        _sniffInterface = args.SniffInterface;
        _showInterface = args.ShowInterface;
        _portFilter = args.PortFilter;
    }

    public async Task StartCapture()
    {
        if (_showInterface)
        {
            PrintDeviceList();
            return;
        }

        ILiveDevice? device = GetDevice(_sniffInterface);
        if (device == null)
        {
            Console.WriteLine("ERROR: interface not found");
            return;
        }
        
        //Set up interface to listen
        device.Open(DeviceModes.Promiscuous);
        if (_portFilter != null) device.Filter = _portFilter;
        
        device.OnPacketArrival += PacketArrivalHandler;
        device.StartCapture();

        CancellationToken token = AsyncManager.Instance.GetCancellationToken();
        while (_packetsGot != _packetMaxNumber && !token.IsCancellationRequested) continue;

        device.StopCapture();
        device.Close();
    }

    private void PacketArrivalHandler(object sender, PacketCapture packet)
    {
        //packet can arrival right before device.StopCapture
        if (_packetsGot == _packetMaxNumber) return;
        if (_parser.ParsePacket(packet.GetPacket()))
        {
            _packetsGot++;
        }
    }

    private static ILiveDevice? GetDevice(string? name)
    {
        if (name == null) return null;
        
        CaptureDeviceList deviceList = CaptureDeviceList.Instance;
        foreach (ILiveDevice device in deviceList)
        {
            //description search is mostly for the windows platform
            if ((device.Description != null && device.Description.Equals(name)) ||
                (device.Name != null && device.Name.Equals(name)))
            {
                return device;
            }
        }

        return null;
    }

    private void PrintDeviceList()
    {
        CaptureDeviceList deviceList = CaptureDeviceList.Instance;
        // Console.WriteLine("Available interfaces:");
        foreach (ILiveDevice device in deviceList)
        {
            Console.WriteLine($"{device.Name}");
        }
    }
}