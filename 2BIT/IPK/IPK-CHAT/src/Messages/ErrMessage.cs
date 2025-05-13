using System.Text;
using System.Text.RegularExpressions;

namespace ipk24chat.Messages;

public class ErrMessage : Message
{
    private string _displayName;
    private string _messageContent;

    private string _pattern = @"ERR\sFROM\s(.*)\sIS\s(.+)";

    //IN-CODE generation(tcp)
    public ErrMessage(string messageContent, byte fillByte)
    {
        Code = 0xFE;
        _messageContent = messageContent;
    }
    
    //IN-CODE generation(udp)
    public ErrMessage(byte[] messageContent, byte fillByte)
    {
        Code = 0xFE;
        //TODO: Convert failed message to an message content
        // _messageContent = messageContent;
    }
    
    //TCP CONSTRUCTOR
    public ErrMessage(string line)
    {
        Code = 0xFE;
        
        Match match = Regex.Match(line, _pattern);

        _displayName = match.Groups[1].Value;
        _messageContent = match.Groups[2].Value;
    }

    //UDP CONSTRUCTOR
    public ErrMessage(byte[] line)
    {
        Code = 0xFE;
        
        if (line.Length < 3 || line[0] != 0xFE)
        {
            //TODO:Invalid ReplyMessage
        }

        MessageId = BitConverter.ToUInt16(line, 1);

        int displayNameEndIndex = Array.IndexOf(line, (byte)0, 3);
        _displayName = Encoding.UTF8.GetString(line, 3, displayNameEndIndex - 2);
        
        int messageContentsEndIndex = Array.IndexOf(line, (byte)0, displayNameEndIndex + 1);
        _messageContent = Encoding.UTF8.GetString(line, displayNameEndIndex + 1, messageContentsEndIndex - displayNameEndIndex - 1);
    }
    
    public override void SetDisplayName(string name)
    {
        _displayName = name;
    }

    public string GetFormattedMessage()
    {
        return "ERR FROM " + _displayName + ": " + _messageContent;
    }
    
    public override char[] EncodeTcp()
    {
        return $"ERR FROM {_displayName} IS {_messageContent}\r\n".ToCharArray();
    }

    public override byte[] EncodeUdp()
    {
        List<byte> bytes = new List<byte>();

        bytes.Add(0xFE);
        bytes.AddRange(BitConverter.GetBytes(MessageId).Reverse());
        bytes.AddRange(Encoding.UTF8.GetBytes(_displayName));
        bytes.Add(0);
        bytes.AddRange(Encoding.UTF8.GetBytes(_messageContent));
        bytes.Add(0);

        return bytes.ToArray();
    }
}