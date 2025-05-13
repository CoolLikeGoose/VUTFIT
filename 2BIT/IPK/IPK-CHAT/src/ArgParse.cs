using Mono.Options;
using System.Net;

namespace ipk24chat;

public class ArgParse
{
    public string? Protocol { get; private set; }
    
    public string? HostName { get; private set; }
    public string GetIp()
    {
        //fix for ::1 address
        if (HostName == "localhost") return "127.0.0.1";
        IPAddress ip = IPAddress.TryParse(HostName, out _)
            ? IPAddress.Parse(HostName)
            : Dns.GetHostEntry(HostName).AddressList[0].MapToIPv4();
        return ip.ToString();
    }
    public ushort HostPort => _hostPort;
    private ushort _hostPort = 4567;
    public ushort UdpTimeout => _udpTimeout;
    private ushort _udpTimeout = 250;
    public byte UdpRetransmissions => _udpRetransmissions;
    private byte _udpRetransmissions = 3;
    
    public ArgParse(string[] args)
    {
        bool printHelp = false;
        OptionSet options = new OptionSet()
        {
            {"t|protocol=", "Transport protocol used for connection(udp/tcp)", x => Protocol = x},
            {"s|hostname=", "Server IP or hostname", x => HostName = x},
            {"p|port=", "Server port", x =>
            {
                if (ushort.TryParse(x, out _hostPort)) return;
                Console.WriteLine("Error: bad port number(must be integer in range [0-65535])");
                Environment.Exit(1);
            }},
            {"d|timeout=", "UDP confirmation timeout", x =>
            {
                if (ushort.TryParse(x, out _udpTimeout)) return;
                Console.WriteLine("Error: bad timeout number(must be integer in range [0-65535])");
                Environment.Exit(1);
            }},
            {"r|retransmissions=", "Maximum number of UDP retransmissions", x =>
            {
                if (byte.TryParse(x, out _udpRetransmissions)) return;
                Console.WriteLine("Error: bad timeout number(must be in range [0-65535])");
                Environment.Exit(1);
            }},
            {"h|help", "Prints program help output and exits", x => printHelp = x != null}
        };

        try
        {
            _ = options.Parse(args);
        }
        catch (OptionException e)
        {
            Console.WriteLine("Error: bad arguments...");
            Console.WriteLine(e.Message);
            Environment.Exit(1);
        }
        
        if (printHelp) PrintHelp(options);

        if (Protocol is not ("tcp" or "udp"))
        {
            Console.WriteLine("Error: protocol must be tcp or udp");
            Environment.Exit(1);
        }

        if (HostName is (null or ""))
        {
            Console.WriteLine("Error: host must be specified");
            Environment.Exit(1);
        }
    }
    
    private static void PrintHelp(OptionSet options)
    {
        Console.WriteLine("ipk24chat-client is a client application, " +
                          "which is able to communicate with a remote server using the IPK24-CHAT protocol");
        Console.WriteLine("Usage: ipk24chat-client -t protocol -s host_name [-p port]" +
                          " [-d udp_timeout] [-r udp_retransmissions] [-h]\n");
        options.WriteOptionDescriptions(Console.Out);
        Environment.Exit(0);
    }
}