using System.Diagnostics;
using ipk24chat.Messages;
using ipk24chat.Network;

namespace ipk24chat;

public class ResponseManager
{
    private static ResponseManager _instance = null;
    private NetworkManager _networkManager;
    
    public static ResponseManager Instance
    {
        get
        {
            if (_instance == null)
            {
                _instance = new ResponseManager();
            }

            return _instance;
        }
    }

    public void SetNetworkManager(NetworkManager networkManager)
    {
        _networkManager = networkManager;
    }
    
    //Maybe merge those functions
    public async Task ProcessNewMessage(byte[] line)
    {
        Message? msg = Message.DecodeMessageUdp(line);
        
        if (msg == null)
        {
            Console.Error.WriteLine("ERR: bad server message");
            await _networkManager.SendMessage(new ErrMessage(line, 0));
            return;
        }
        await ProcessTypedMessage(msg);
    }

    public async Task ProcessNewMessage(string line)
    {
        Message? msg = Message.DecodeMessageTcp(line);
        if (msg == null)
        {
            Console.Error.WriteLine("ERR: bad server message");
            await _networkManager.SendMessage(new ErrMessage(line, 0));
            return;
        }

        await ProcessTypedMessage(msg);
    }

    private async Task ProcessTypedMessage(Message msg)
    {
        if (msg.GetMessageType() != 0x00) 
            _networkManager.SendConfirm(msg.GetMessageId());
        switch (msg.GetMessageType())
        {
            case 0x00:
                await ProcessConfirm((ConfirmMessage)msg);
                break;
            case 0x01:
                await ProcessReply((ReplyMessage)msg);
                break;
            case 0x04:
                ProcessMessage((SentMessage)msg);
                break;    
            case 0xFE:
                ProcessError((ErrMessage)msg);
                break;
            case 0xFF:
                ProcessBye((ByeMessage)msg);
                break;
        }
    }

    private async Task ProcessReply(ReplyMessage msg)
    {
        if (msg.GetStatus())
        {
            Console.Error.WriteLine($"Success: {msg.GetMessage()}");
        }
        else
        {
            Console.Error.WriteLine($"Failure: {msg.GetMessage()}");
        }
        AsyncManager.Instance.ReleaseReplyAwait(msg.GetStatus());
    }

    private async Task ProcessConfirm(ConfirmMessage msg)
    {
        _networkManager.AcceptConfirm(msg.GetMessageId());
    }

    private void ProcessMessage(SentMessage msg)
    {
        Console.WriteLine(msg.GetFormattedMessage());
    }

    private void ProcessError(ErrMessage msg)
    {
        Console.Error.WriteLine(msg.GetFormattedMessage());
        AsyncManager.Instance.SendCancellationCall();
    }

    private void ProcessBye(ByeMessage msg)
    {
        AsyncManager.Instance.SendCancellationCall();
    }
}