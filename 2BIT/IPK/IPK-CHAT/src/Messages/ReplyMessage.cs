using System.Text;
using System.Text.RegularExpressions;

namespace ipk24chat.Messages;

public class ReplyMessage : Message
{
    private string pattern = @"REPLY\s(OK|NOK)\sIS\s(.+)";

    private bool _status = false;
    private ushort _refMessage;
    private string _message;

    public bool GetStatus()
    {
        return _status;
    }

    public string GetMessage()
    {
        return _message;
    }
    
    //UDP constructor
    public ReplyMessage(byte[] line)
    {
        Code = 0x01;
        if (line.Length < 7 || line[0] != 0x01)
        {
            //TODO:Invalid ReplyMessage
            return;
        }

        MessageId = BitConverter.ToUInt16(line, 1);
        _status = line[3] != 0;
        _refMessage = BitConverter.ToUInt16(line, 4);
        int nullByteIndex = Array.IndexOf(line, (byte)0, 6);
        _message = nullByteIndex != -1 ? 
            System.Text.Encoding.ASCII.GetString(line, 6, nullByteIndex - 6) : 
            System.Text.Encoding.ASCII.GetString(line, 6, line.Length - 7);
    }
    
    //TCP constructor
    public ReplyMessage(string line)
    {
        Code = 0x01;
        
        Match match = Regex.Match(line, pattern);

        if (match.Groups[1].Value == "OK") _status = true;
        else _status = false;

        _message = match.Groups[2].Value;
    }
}