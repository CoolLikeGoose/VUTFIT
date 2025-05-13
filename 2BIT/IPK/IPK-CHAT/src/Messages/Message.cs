using System.Text;

namespace ipk24chat.Messages;

public abstract class Message
{
    protected byte Code;
    protected ushort MessageId;

    public void SetMessageId(ushort messageId)
    {
        MessageId = messageId;
    }
    
    public ushort GetMessageId()
    {
        return MessageId;
    }

    public virtual void SetDisplayName(string name)
    {
        return;
    }

    public byte GetMessageType()
    {
        return Code;
    }

    public virtual byte[] EncodeUdp()
    {
        return Array.Empty<byte>();
    }

    public virtual char[] EncodeTcp()
    {
        return Array.Empty<char>();
    }

    public static Message? DecodeMessageUdp(byte[] msg)
    {
        if (msg.Length < 3)
        {
            return null;
        }
        
        switch (msg[0])
        {
            case 0x00:
                return new ConfirmMessage(msg);
            case 0x01:
                return new ReplyMessage(msg);
            case 0x04:
                return new SentMessage(msg);
            case 0xFE:
                return new ErrMessage(msg);
            case 0xFF:
                return new ByeMessage(msg);
        }

        return null;
    }

    public static Message? DecodeMessageTcp(string msg)
    {
        switch (msg.Split()[0])
        {
            case "REPLY":
                return new ReplyMessage(msg);
            case "MSG":
                return new SentMessage(msg);
            case "ERR":
                return new ErrMessage(msg);
            case "BYE":
                return new ByeMessage();
        }
        return null;
    }
}