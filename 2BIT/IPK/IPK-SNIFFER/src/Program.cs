namespace  ipk_sniffer;

internal static class Program
{
    public static async Task Main(string[] args)
    {
        ArgParse arguments = new ArgParse(args);
        Console.CancelKeyPress += new ConsoleCancelEventHandler(CancelKeyPressHandler);

        PacketSniffer packetSniffer = new PacketSniffer(arguments);
        Task catcherTask = Task.Run(packetSniffer.StartCapture);
        AsyncManager.Instance.SubscribeTaskToCancellation(catcherTask);
        
        await AsyncManager.Instance.WaitUntilTasksEnds();
    }

    private static async void CancelKeyPressHandler(object? sender, ConsoleCancelEventArgs args)
    {
        AsyncManager.Instance.SendCancellationCall();
        await AsyncManager.Instance.WaitUntilTasksEnds();
        Environment.Exit(0);
    }
}