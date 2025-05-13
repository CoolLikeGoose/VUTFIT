using StudIS.APP.ViewModels;

namespace StudIS.APP.Views;

public partial class ContentPageBase
{
    private IViewModel ViewModel { get; }
    
    public ContentPageBase(IViewModel viewModel)
    {
        InitializeComponent();

        ViewModel = viewModel;
        BindingContext = viewModel;
    }
    
    protected override async void OnAppearing()
    {
        base.OnAppearing();
        
        await ViewModel.LoadDataAsync();
    }
}