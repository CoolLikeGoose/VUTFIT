using Mono.Options;

namespace ipk_sniffer;

public class ArgParse
{
    public bool ShowInterface = false;
    public string? SniffInterface;
    public bool Tcp = false;
    public bool Udp = false;
    private int _port = -1;
    private int _portDst = -1;
    private int _portSrc = -1;
    public string? PortFilter;
    public bool Icmp4 = false;
    public bool Icmp6 = false;
    public bool Arp = false;
    public bool Ndp = false;
    public bool Igmp = false;
    public bool Mld = false;
    public int NumberOfPackets = 1;
    
    public ArgParse(string[] args)
    {
        bool printHelp = false;
        OptionSet options = new OptionSet()
        {
            {"h|help", "Prints program help output and exits", x => printHelp = x != null},
            {"i|interface=", "Interface to sniff", x => SniffInterface = x },
            {"t|tcp", "Display TCP segments and is optionally complemented by -p", x => Tcp = x != null},
            {"u|udp", "Display Udp segments and is optionally complemented by -p", x => Udp = x != null},
            {"p|port=", "Filter TCP/UDP based on port number", x =>
            {
                if (int.TryParse(x, out _port)) return;
                if (_port <= 65535 || _port >= 0) return;
                Console.WriteLine("Error: bad port number(must be integer in range [0-65535])");
                Environment.Exit(1);
            }},
            {"port-destination=", "Filter TCP/UDP based on port number", x =>
            {
                if (int.TryParse(x, out _port)) return;
                if (_portDst <= 65535 || _port >= 0) return;
                Console.WriteLine("Error: bad port number(must be integer in range [0-65535])");
                Environment.Exit(1);
            }},
            {"port-source=", "Filter TCP/UDP based on port number", x =>
            {
                if (int.TryParse(x, out _port)) return;
                if (_portSrc <= 65535 || _port >= 0) return;
                Console.WriteLine("Error: bad port number(must be integer in range [0-65535])");
                Environment.Exit(1);
            }},
            {"icmp4", "Will display only ICMPv4 packets", x => Icmp4 = x != null},
            {"icmp6", "Will display only ICMPv6 echo request/response", x => Icmp6 = x != null},
            {"arp", "Will display only ARP frames", x => Arp = x != null},
            {"ndp", "Will display only NDP packets, subset of ICMPv6", x => Ndp = x != null},
            {"igmp", "Will display only IGMP packets", x => Igmp = x != null},
            {"mld", "Will display only MLD packets, subset of ICMPv6", x => Mld = x != null},
            {"n=", "Specifies the number of packets to display", x =>
            {
                if (int.TryParse(x, out NumberOfPackets)) return;
                if (NumberOfPackets > 0) return;
                Console.WriteLine("Error: bad port number(must be integer in range [0-65535])");
                Environment.Exit(1);
            }}
        };

        List<string>? extra = null;
        try
        {
            extra = options.Parse(args);
        }
        catch (OptionException e)
        {
            //OptionSet cant handle situation when the flag has an optional value
            if (e.OptionName != "-i")
            {
                Console.WriteLine("Error: bad arguments...");
                Console.WriteLine(e.Message);
                Environment.Exit(1);
            }
        }
        
        if (printHelp) PrintHelp(options);
        
        if (SniffInterface == null)
        {
            ShowInterface = true;
        }

        if (!Tcp && !Udp && !Icmp4 && !Icmp6 && !Arp && !Ndp && !Igmp && !Mld)
        {
            Tcp = true;
            Udp = true;
            Icmp4 = true;
            Icmp6 = true;
            Arp = true;
            Ndp = true;
            Igmp = true;
            Mld = true;
        }

        if (extra == null) return;
        
        if (args.Contains("-p"))
        {
            PortFilter = $"port {_port}";
        } 
        if (args.Contains("--port-destination"))
        {
            if (PortFilter != "") PortFilter += " or ";
            PortFilter = $"dst port {_portDst}";
        } 
        if (args.Contains("--port-source"))
        {
            if (PortFilter != "") PortFilter += " or ";
            PortFilter = $"src port {_portSrc}";
        }
    }
    
    private static void PrintHelp(OptionSet options)
    {
        Console.WriteLine("ipk sniffer is a network traffic monitoring application.");
        Console.WriteLine("./ipk-sniffer [-i interface | --interface interface]" +
                          " {-p|--port-source|--port-destination port [--tcp|-t] [--udp|-u]}" +
                          " [--arp] [--ndp] [--icmp4] [--icmp6] [--igmp] [--mld] {-n num}\n");
        options.WriteOptionDescriptions(Console.Out);
        Environment.Exit(0);
    }
}