using ipk24chat.Messages;

namespace ipk24chat.Network;

public interface IProtocolHandler
{
    public void InitConnection(string host, ushort port);
    public Task SendMessage(Message message);
    public Task ReceiveMessage();
    public void CloseConnection();

    public void AcceptConfirm(ushort messageId);
    public void SendConfirm(ushort messageId);
}