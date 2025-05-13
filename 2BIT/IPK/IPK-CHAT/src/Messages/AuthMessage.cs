using System.Text;

namespace ipk24chat.Messages;

public class AuthMessage : Message
{
    private string _userName;
    private string _displayName;
    private string _secret;
    
    public AuthMessage(string userName, string secret)
    {
        Code = 0x02;
        _userName = userName;
        _secret = secret;
    }
    
    public override void SetDisplayName(string name)
    {
        _displayName = name;
    }

    public override char[] EncodeTcp()
    {
        return $"AUTH {_userName} AS {_displayName} USING {_secret}\r\n".ToCharArray();
    }

    public override byte[] EncodeUdp()
    {
        List<byte> bytes = new List<byte>();

        bytes.Add(0x02);
        bytes.AddRange(BitConverter.GetBytes(MessageId).Reverse());

        bytes.AddRange(Encoding.ASCII.GetBytes(_userName));
        bytes.Add(0);

        bytes.AddRange(Encoding.ASCII.GetBytes(_displayName));
        bytes.Add(0);
        
        bytes.AddRange(Encoding.ASCII.GetBytes(_secret));
        bytes.Add(0);
        
        

        return bytes.ToArray();
    }
}