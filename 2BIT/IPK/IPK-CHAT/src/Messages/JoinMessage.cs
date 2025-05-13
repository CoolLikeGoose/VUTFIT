using System.Text;

namespace ipk24chat.Messages;

public class JoinMessage : Message
{
    private string _channelId;
    private string _displayedName;
    
    public JoinMessage(string channelId)
    {
        Code = 0x03;
        _channelId = channelId;
    }

    public override void SetDisplayName(string name)
    {
        _displayedName = name;
    }

    public override char[] EncodeTcp()
    {
        return $"JOIN {_channelId} AS {_displayedName}\r\n".ToCharArray();
    }

    public override byte[] EncodeUdp()
    {
        List<byte> bytes = new List<byte>();

        bytes.Add(0x03);
        bytes.AddRange(BitConverter.GetBytes(MessageId).Reverse());
        bytes.AddRange(Encoding.ASCII.GetBytes(_channelId));
        bytes.Add(0);
        bytes.AddRange(Encoding.ASCII.GetBytes(_displayedName));
        bytes.Add(0);

        return bytes.ToArray();
    }
}