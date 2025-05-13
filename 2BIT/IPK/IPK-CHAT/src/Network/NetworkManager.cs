using System.Text;
using ipk24chat.Messages;

namespace ipk24chat.Network;

public class NetworkManager
{
    private IProtocolHandler _protocol;
    private ushort _messageId = 0;
    private string _displayedName = "";
    
    public NetworkManager(string? protocol, string host, ushort port, ushort udpTimeout, byte udpRetransmissions)
    {
        if (protocol == "udp")
        {
            _protocol = new UdpHandler(udpTimeout, udpRetransmissions);
        }
        else
        {
            _protocol = new TcpHandler();
        }
        
        _protocol.InitConnection(host, port); 
    }

    public void ChangeDisplayName(string name)
    {
        _displayedName = name;
    }

    public async Task SendMessage(Message? message)
    {
        if (message == null) return;  
        
        await AsyncManager.Instance.WaitForSend();
        
        message?.SetDisplayName(_displayedName);
        message?.SetMessageId(_messageId++);
        await _protocol.SendMessage(message);

        AsyncManager.Instance.ReleaseSendAwait();
    }

    public void AcceptConfirm(ushort messageId)
    {
        _protocol.AcceptConfirm(messageId);
    }
    
    public void SendConfirm(ushort messageId)
    {
        _protocol.SendConfirm(messageId);   
    }

    public async Task SendBye()
    {
        await AsyncManager.Instance.WaitForSend();

        Message byeMsg = new ByeMessage();
        await _protocol.SendMessage(byeMsg);
        
        AsyncManager.Instance.ReleaseSendAwait();
    }

    public void CloseConnection()
    {
        _protocol.CloseConnection();
    }
}