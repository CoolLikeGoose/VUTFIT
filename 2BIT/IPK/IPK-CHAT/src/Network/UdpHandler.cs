using System.Collections.Concurrent;
using System.Net;
using System.Net.Sockets;
using System.Text;
using ipk24chat.Messages;

namespace ipk24chat.Network;

public class UdpHandler : IProtocolHandler
{
    private UdpClient _client;

    private readonly TimeSpan _delayBetweenRetries;
    private readonly byte _udpRetransmissions;

    private ConcurrentDictionary<ushort, TaskCompletionSource> _tasksInProcess;
    private ConcurrentBag<ushort> _taskCompleted;

    public UdpHandler(ushort udpTimeout, byte udpRetransmissions)
    {
        _udpRetransmissions = udpRetransmissions;
        _delayBetweenRetries = TimeSpan.FromMilliseconds(udpTimeout);

        _tasksInProcess = new ConcurrentDictionary<ushort, TaskCompletionSource>();
        _taskCompleted = new ConcurrentBag<ushort>();
    }
    
    public void InitConnection(string? host, ushort port)
    {
        _client = new UdpClient();

        try
        {
            IPEndPoint server = new IPEndPoint(IPAddress.Parse(host), port);
            _client.Connect(server);
            
            Task receiveTask = Task.Run(ReceiveMessage);
            AsyncManager.Instance.SubscribeTaskToCancellation(receiveTask);
        }
        catch (Exception e)
        {
            Console.Error.WriteLine($"ERR: {e.Message}");
            Environment.Exit(5);
        }
    }

    //Async function that implements packets re-send in packet loss case
    private async Task UdpConfirmationWaiter(Message message, TaskCompletionSource sentTask)
    {
        int curRetry = 0;
        while (curRetry < _udpRetransmissions)
        {
            Task delayTask = Task.Delay(_delayBetweenRetries);
            Task completedTask = await Task.WhenAny(delayTask, sentTask.Task);

            if (completedTask == sentTask.Task)
            {
                _tasksInProcess.TryRemove(message.GetMessageId(), out sentTask);
                _taskCompleted.Add(message.GetMessageId());
                return;
            }
            curRetry++;
            await SendMessageAgain(message);
        }
        
        await Console.Error.WriteLineAsync($"ERR: failed to send message {message.GetMessageId()}");
    }
    
    //Re-send function
    private async Task SendMessageAgain(Message message)
    {
        byte[] convertedMsg = message.EncodeUdp();
        await _client.SendAsync(convertedMsg, convertedMsg.Length);
    }

    public async Task SendMessage(Message message)
    {
        byte[] convertedMsg = message.EncodeUdp();
        await _client.SendAsync(convertedMsg, convertedMsg.Length);
        
        TaskCompletionSource tsk = new TaskCompletionSource();
        _tasksInProcess.TryAdd(message.GetMessageId(), tsk);
        Task.Run(() => UdpConfirmationWaiter(message, tsk));
    }

    public void AcceptConfirm(ushort messageId)
    {
        if (_tasksInProcess.ContainsKey(messageId))
        {
            _tasksInProcess.TryGetValue(messageId, out var tsk);
            tsk?.SetResult();
        }
        //else msg must be in _completed or doenst exist
    }

    public async void SendConfirm(ushort messageId)
    {
        await SendMessageAgain(new ConfirmMessage(messageId));
    }


    public async Task ReceiveMessage()
    {
        CancellationToken token = AsyncManager.Instance.GetCancellationToken();
        while (!token.IsCancellationRequested)
        {
            try
            {
                UdpReceiveResult result = await _client.ReceiveAsync(token);
                await ResponseManager.Instance.ProcessNewMessage(result.Buffer);
            }
            catch (Exception e)
            {
                break;
            }
        }
    }


    public void CloseConnection()
    {
        _client.Close();
    }
}