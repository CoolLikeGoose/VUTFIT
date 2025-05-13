namespace ipk_sniffer;

public class AsyncManager
{
    private static AsyncManager? _instance = null;
    private CancellationTokenSource _token;
    private List<Task> _subscribedTasks;
    private bool _initedExit = false;
    
    private AsyncManager()
    {
        _token = new CancellationTokenSource();
        _subscribedTasks = new List<Task>();
    }
    
    public static AsyncManager Instance
    {
        get
        {
            if (_instance == null)
            {
                _instance = new AsyncManager();
            }

            return _instance;
        }
    }
    
    public void SubscribeTaskToCancellation(Task task)
    {
        _subscribedTasks.Add(task);
    }
    
    public async void SendCancellationCall()
    {
        await _token.CancelAsync();
    }
    
    public CancellationToken GetCancellationToken()
    {
        return _token.Token;
    }
    
    public async Task WaitUntilTasksEnds()
    {
        await Task.WhenAll(_subscribedTasks);
    }
}