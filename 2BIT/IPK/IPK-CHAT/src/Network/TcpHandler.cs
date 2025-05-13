using System.Net.Sockets;
using ipk24chat.Messages;

namespace ipk24chat.Network;

public class TcpHandler: IProtocolHandler
{
    private TcpClient? _client;

    // private NetworkStream _stream;
    private StreamWriter _writeStream;
    private StreamReader _readStream;

    public void InitConnection(string? host, ushort port)
    {
        _client = new TcpClient();
        try
        {
            _client.Connect(host, port);
            _writeStream = new StreamWriter(_client.GetStream());
            _readStream = new StreamReader(_client.GetStream());
            
            Task receiveTask =  Task.Run(ReceiveMessage);
            AsyncManager.Instance.SubscribeTaskToCancellation(receiveTask);
        }
        catch (Exception e)
        {
            Console.Error.WriteLine($"ERR: {e.Message}");
            Environment.Exit(5);
        }
    }

    public async Task SendMessage(Message message)
    {
        await _writeStream.WriteAsync(message.EncodeTcp());
        await _writeStream.FlushAsync();
    }

    public async Task ReceiveMessage()
    {
        CancellationToken token = AsyncManager.Instance.GetCancellationToken();
        while (!token.IsCancellationRequested)
        {
            try
            {
                string? msg = await _readStream.ReadLineAsync(token);
                if (!string.IsNullOrEmpty(msg)) await ResponseManager.Instance.ProcessNewMessage(msg);
            }
            catch (Exception e)
            {
                break;
            }
        }
    }

    public void CloseConnection()
    {
        _writeStream?.Close();
        _readStream?.Close();
    }

    public void AcceptConfirm(ushort messageId)
    {
        //Confirm in TCP?
        return;
    }
    
    public void SendConfirm(ushort messageId)
    {
        //Confirm in TCP?
        return;
    }
}