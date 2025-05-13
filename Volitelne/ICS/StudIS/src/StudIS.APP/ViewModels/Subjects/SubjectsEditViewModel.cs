using System.ComponentModel;
using System.Runtime.CompilerServices;
using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using StudIS.BL.Facades.Interfaces;
using StudIS.BL.Models;

namespace StudIS.APP.ViewModels.Subjects;

public partial class SubjectsEditViewModel : ObservableObject, IViewModel, IQueryAttributable
{
    private readonly ISubjectFacade _subjectFacade;
    private Guid _subjectId;

    public SubjectsEditViewModel(ISubjectFacade subjectFacade)
    {
        _subjectFacade = subjectFacade;
    }

    private SubjectDetailModel _subject;
    public SubjectDetailModel Subject
    {
        get => _subject;
        set => SetProperty(ref _subject, value);
    }

    public async Task LoadDataAsync()
    {
        if (_subjectId != Guid.Empty)
        {
            Subject = await _subjectFacade.GetAsync(_subjectId);
        }
        else
        {
            Subject = new SubjectDetailModel
            {
                Name = "Default Name", // Default value for Name
                Abbreviation = "DEF"  // Default value for Abbreviation
            };
        }
    }

    public async void ApplyQueryAttributes(IDictionary<string, object> query)
    {
        if (query.ContainsKey("Id"))
        {
            _subjectId = (Guid)query["Id"];
        }
        await LoadDataAsync();
    }

    [RelayCommand]
    private async Task SaveAsync()
    {
        await _subjectFacade.SaveAsync(Subject);
        await Shell.Current.GoToAsync("..");
    }

    [RelayCommand]
    private async Task CancelAsync()
    {
        await Shell.Current.GoToAsync("..");
    }
}