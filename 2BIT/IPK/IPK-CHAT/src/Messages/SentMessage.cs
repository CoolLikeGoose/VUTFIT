using System.Text;
using System.Text.RegularExpressions;

namespace ipk24chat.Messages;

public class SentMessage : Message
{
    private string _displayName;
    private string _messageContent;

    private string _pattern = @"MSG\sFROM\s(.*)\sIS\s(.+)";
    
    //IN-CODE generation
    public SentMessage(string messageContent, byte fillByte)
    {
        Code = 0x04;
        _messageContent = messageContent;
    }

    //TCP CONSTRUCTOR
    public SentMessage(string line)
    {
        Code = 0x04;
        
        Match match = Regex.Match(line, _pattern);

        _displayName = match.Groups[1].Value;
        _messageContent = match.Groups[2].Value;
    }

    //UDP CONSTRUCTOR
    public SentMessage(byte[] line)
    {
        Code = 0x04;
        
        if (line.Length < 5 || line[0] != 0x04)
        {
            //TODO:Invalid ReplyMessage
        }
        
        MessageId = BitConverter.ToUInt16(line, 1);
        
        int displayNameEndIndex = Array.IndexOf(line, (byte)0, 3);
        _displayName = Encoding.ASCII.GetString(line, 3, displayNameEndIndex - 3);

        // Find null terminator for MessageContents
        int messageContentsStartIndex = displayNameEndIndex + 1;
        int messageContentsEndIndex = Array.IndexOf(line, (byte)0, messageContentsStartIndex);

        _messageContent = Encoding.UTF8.GetString(line, messageContentsStartIndex, messageContentsEndIndex - messageContentsStartIndex);
    }

    public override void SetDisplayName(string name)
    {
        _displayName = name;
    }

    public string GetFormattedMessage()
    {
        return _displayName + ": " + _messageContent;
    }

    public override char[] EncodeTcp()
    {
        return $"MSG FROM {_displayName} IS {_messageContent}\r\n".ToCharArray();
    }

    public override byte[] EncodeUdp()
    {
        List<byte> bytes = new List<byte>();

        bytes.Add(0x04);
        bytes.AddRange(BitConverter.GetBytes(MessageId).Reverse());
        bytes.AddRange(Encoding.ASCII.GetBytes(_displayName));
        bytes.Add(0);
        bytes.AddRange(Encoding.ASCII.GetBytes(_messageContent));
        bytes.Add(0);

        return bytes.ToArray();
    }
}