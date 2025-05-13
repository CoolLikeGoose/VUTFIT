using System.ComponentModel;
using System.Runtime.CompilerServices;
using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using StudIS.BL.Facades.Interfaces;
using StudIS.BL.Models;
using System.Collections.ObjectModel;

namespace StudIS.APP.ViewModels.Subjects;

public partial class SubjectsDetailViewModel : ObservableObject, IViewModel, IQueryAttributable, INotifyPropertyChanged
{
    private readonly ISubjectFacade _subjectFacade;
    private Guid Id { get; set; }

    public SubjectsDetailViewModel(ISubjectFacade subjectFacade)
    {
        _subjectFacade = subjectFacade;
    }

    private SubjectDetailModel _subject;
    public SubjectDetailModel Subject
    {
        get => _subject;
        set => SetProperty(ref _subject, value);
    }

    public ObservableCollection<ActivityListModel> Activities { get; } = new();

    public async Task LoadDataAsync()
    {
        Subject = await _subjectFacade.GetAsync(Id);
        OnPropertyChanged(nameof(Subject));

        Activities.Clear();
        foreach (var activity in Subject.Activities)
        {
            Activities.Add(activity);
        }
    }

    public async void ApplyQueryAttributes(IDictionary<string, object> query)
    {
        if (query.ContainsKey("Id"))
        {
            Id = (Guid)query["Id"];
        }
        await LoadDataAsync();
    }

    [RelayCommand]
    private async Task DeleteAsync()
    {
        await _subjectFacade.DeleteAsync(Id);
        await Shell.Current.GoToAsync("..");
    }

    [RelayCommand]
    private async Task EditAsync()
    {
        await Shell.Current.GoToAsync("edit", new Dictionary<string, object> { { "Id", Id } });
    }

    [RelayCommand]
    private async Task GoToActivityDetailAsync(Guid id)
    {
        await Shell.Current.GoToAsync("//activities/detail", new Dictionary<string, object> { { "Id", id } });
    }

    [RelayCommand]
    private async Task GoToCreateActivityAsync()
    {
        await Shell.Current.GoToAsync("//activities/edit");
    }

    public event PropertyChangedEventHandler? PropertyChanged;

    protected virtual void OnPropertyChanged([CallerMemberName] string? propertyName = null)
    {
        PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
    }
}
