namespace ipk24chat.Messages;

public class ConfirmMessage : Message
{
    public ConfirmMessage(ushort refMessage)
    {
        Code = 0x00;
        MessageId = refMessage;
    }
    
    public ConfirmMessage(byte[] line)
    {
        Code = 0x00;
        
        if (line.Length < 3 || line[0] != 0xFF)
        {
            //TODO:Invalid ReplyMessage
        }

        MessageId = BitConverter.ToUInt16(line, 1);
    }

    public override byte[] EncodeUdp()
    {
        List<byte> bytes = new List<byte>();

        bytes.Add(0x00);
        bytes.AddRange(BitConverter.GetBytes(MessageId));

        return bytes.ToArray();
    }
}