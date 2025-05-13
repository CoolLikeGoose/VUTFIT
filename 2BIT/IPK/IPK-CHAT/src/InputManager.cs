using ipk24chat.Messages;
using ipk24chat.Network;

namespace ipk24chat;

public class InputManager
{
    private readonly NetworkManager _networkManager;
    //user information
    private ushort _messageId = 0;
    private bool auth_completed = false;
    
    
    public InputManager(NetworkManager manager)
    {
        _networkManager = manager;
    }

    public async Task StartUserRead()
    {
        CancellationToken token = AsyncManager.Instance.GetCancellationToken();
        using StreamReader reader = new StreamReader(Console.OpenStandardInput());
        //TODO: Check for possible problems
        bool response = false;
        try
        {
            while (!token.IsCancellationRequested)
            {
                string? line = await reader.ReadLineAsync(token);
                if (line == null)
                {
                    //IF stdin closed
                    AsyncManager.Instance.SendCancellationCall();
                    break;
                }
                Message? msg = ParseLine(line.Trim());
                await _networkManager.SendMessage(msg);

                if (msg?.GetMessageType() == 0x02 || msg?.GetMessageType() == 0x03)
                {
                    response = await AsyncManager.Instance.WaitForResponse();
                    if (msg?.GetMessageType() == 0x02 && !response) auth_completed = false;
                }
                    
            }
        }
        catch (Exception e) {}
    }
    
    private Message? ParseLine(string line)
    {
        if (line.Length == 0)
        {
            PrintError("You cant send empty line");
            return null;
        }
        if (line.Trim()[0] == '/') 
            return ProcessCommand(line);
        return ProcessMessage(line);
            
    }
    
    private Message? ProcessCommand(string line)
    {
        string[] command = line.Split();

        if (command[0] == "/auth") return ProcessAuth(command);
        if (command[0] == "/join") return ProcessJoin(command);
        if (command[0] == "/rename") return ProcessRename(command);
        if (command[0] == "/help") return PrintHelp();
        
        PrintError("Unknown command");
        return null;
    }

    private Message? ProcessAuth(string[] command)
    {
        if (auth_completed)
        {
            PrintError("Auth already completed");
            return null;
        }
        if (command.Length != 4 ||
            command[1].Length > 20 ||
            command[2].Length > 128 ||
            command[3].Length > 20)
        {
            PrintError("Wrong arguments");
            return null;
        }

        auth_completed = true;
        _networkManager.ChangeDisplayName(command[3]);
        return new AuthMessage(
            command[1], //user name
            command[2]); //secret
    }

    private Message? ProcessJoin(string[] command)
    {
        if (!auth_completed)
        {
            PrintError("You need to authenticate with /auth");
            return null;
        }
        if (command.Length != 2 ||
            command[1].Length > 20)
        {
            PrintError("Wrong arguments");
            return null;
        } 
        
        return new JoinMessage(command[1]);
            
    }
    
    private Message? ProcessRename(string[] command)
    {
        if (command.Length != 2 ||
            command[1].Length > 20)
        {
            PrintError("Wrong arguments");
            return null;
        }

        _networkManager.ChangeDisplayName(command[1]);
        return null;
    }

    private Message? ProcessMessage(string line)
    {
        if (!auth_completed)
        {
            PrintError("You need to authenticate with /auth (more info in /help)");
            return null;
        }
        if (line.Length > 1400)
        {
            PrintError("Too big message");
            return null;
        }

        return new SentMessage(line, 0);
    }

    private void PrintError(string errorMsg)
    {
        Console.Error.WriteLine($"ERR: {errorMsg}. Type /help for additional information");
    }

    private Message? PrintHelp()
    {
        Console.WriteLine("Commands available on server:");
        Console.WriteLine("/auth {Username} {Secret} {DisplayName}\tAuthentication on the server");
        Console.WriteLine("/join {ChannelID}\tJoin channel");
        Console.WriteLine("/rename {DisplayName}\tChange your name on server");
        Console.WriteLine("/help\tPrint this information");
        return null;
    }
}