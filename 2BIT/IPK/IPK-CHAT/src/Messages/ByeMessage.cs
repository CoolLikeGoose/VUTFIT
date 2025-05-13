namespace ipk24chat.Messages;

public class ByeMessage : Message
{
    //Generation for TCP
    public ByeMessage()
    {
        Code = 0xFF;
    }

    public ByeMessage(byte[] line)
    {
        if (line.Length < 3 || line[0] != 0xFF)
        {
            //TODO:Invalid ReplyMessage
        }

        MessageId = BitConverter.ToUInt16(line, 1);
    }
    
    public override char[] EncodeTcp()
    {
        return "BYE\r\n".ToCharArray();
    }
    
    public override byte[] EncodeUdp()
    {
        List<byte> bytes = new List<byte>();

        bytes.Add(0xFF);
        bytes.AddRange(BitConverter.GetBytes(MessageId));

        return bytes.ToArray();
    }
}