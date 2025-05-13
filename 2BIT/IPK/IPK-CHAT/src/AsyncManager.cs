using System.Collections.Concurrent;

namespace ipk24chat;

public class AsyncManager
{
    private static AsyncManager? _instance = null;
    private CancellationTokenSource _token;
    private List<Task> _subscribedTasks;
    private bool _initedExit;

    private SemaphoreSlim _semaphoreSendAwait = new SemaphoreSlim(1);
    
    private SemaphoreSlim _semaphoreReplyAwait = new SemaphoreSlim(0);
    private bool _lastResponse;
    
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
        _semaphoreSendAwait.Release();
        _semaphoreReplyAwait.Release();
        await WaitUntilTasksEnds();
    }

    public CancellationToken GetCancellationToken()
    {
        return _token.Token;
    }

    public async Task WaitUntilTasksEnds()
    {
        await Task.WhenAll(_subscribedTasks);
    }

    public async Task<bool> WaitForResponse()
    {
        await _semaphoreReplyAwait.WaitAsync();
        return _lastResponse;
    }

    public void ReleaseReplyAwait(bool response)
    {
        _lastResponse = response;
        _semaphoreReplyAwait.Release();
    }

    public async Task WaitForSend()
    {
        await _semaphoreSendAwait.WaitAsync();
    }
    
    public void ReleaseSendAwait()
    {
        _semaphoreSendAwait.Release();
    }
}