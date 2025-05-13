using ipk24chat.Network;

namespace ipk24chat;

internal static class Program
{
    private static NetworkManager? _networkManager;
    public static async Task Main(string[] args)
    {
        ArgParse arguments = new ArgParse(args);
        Console.CancelKeyPress += new ConsoleCancelEventHandler(CancelKeyPressHandler);

        _networkManager = new NetworkManager(
            arguments.Protocol?.ToLower(),
            arguments.GetIp(),
            arguments.HostPort,
            arguments.UdpTimeout,
            arguments.UdpRetransmissions);

        ResponseManager.Instance.SetNetworkManager(_networkManager);
        InputManager inputManager = new InputManager(_networkManager);
        
        Task inputTask = inputManager.StartUserRead();
        AsyncManager.Instance.SubscribeTaskToCancellation(inputTask);
        
        await AsyncManager.Instance.WaitUntilTasksEnds();
        await _networkManager.SendBye();
        _networkManager.CloseConnection();
    }
    
    private static async void CancelKeyPressHandler(object? sender, ConsoleCancelEventArgs args)
    {
        AsyncManager.Instance.SendCancellationCall();
        await _networkManager.SendBye();
        _networkManager?.CloseConnection();
        Environment.Exit(0);
    }
}