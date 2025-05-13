using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using StudIS.BL.Facades;
using StudIS.BL.Facades.Interfaces;
using StudIS.BL.Models;

namespace StudIS.APP.ViewModels.Student;

public partial class StudentDetailViewModel(IStudentFacade studentFacade, ISubjectFacade subjectFacade) : ObservableObject, IViewModel, IQueryAttributable, INotifyPropertyChanged
{
    private Guid Id { get; set; }

    private StudentDetailModel _student;


    public StudentDetailModel Student
    {
        get => _student;
        set => SetProperty(ref _student, value);
    }
    
    public async Task LoadDataAsync()
    {
        Student = await studentFacade.GetAsync(Id);
        OnPropertyChanged("Student");

        IsAddButtonVisible = true;
        IsPickerVisible = false;
        IsSaveButtonVisible = false;

        IEnumerable<SubjectListModel> subjects = await subjectFacade.GetAsync();
        AvailableSubjects.Clear();
        foreach (SubjectListModel subject in subjects)
        {
            if (!Student.Subjects.Any(s => s.SubjectId == subject.Id))
            {
                AvailableSubjects.Add(subject);
            }
        }
    }

    public async void ApplyQueryAttributes(IDictionary<string, object> query)
    {
        Id = (Guid)query["Id"];
        await LoadDataAsync();
    }

    [RelayCommand]
    private async Task DeleteAsync()
    {
        await studentFacade.DeleteAsync(Id);
        await Shell.Current.GoToAsync("..");
    }

    [RelayCommand]
    private async Task EditAsync()
    {
        await Shell.Current.GoToAsync("edit", new Dictionary<string, object> { { "Id", Id } });
    }

    [RelayCommand]
    private async Task GoToDetailAsync(Guid id)
    {
        await Shell.Current.GoToAsync("//subjects/detail", new Dictionary<string, object> { { "Id", id } });
    }

    //Add subject section
    private bool _isPickerVisible;
    public bool IsPickerVisible
    {
        get => _isPickerVisible;
        set
        {
            SetProperty(ref _isPickerVisible, value);
            OnPropertyChanged();
        }
    }

    private bool _isSaveButtonVisible;
    public bool IsSaveButtonVisible
    {
        get => _isSaveButtonVisible;
        set
        {
            SetProperty(ref _isSaveButtonVisible, value);
            OnPropertyChanged();
        }
    }

    private bool _isAddButtonVisible;
    public bool IsAddButtonVisible
    {
        get => _isAddButtonVisible;
        set
        {
            SetProperty(ref _isAddButtonVisible, value);
            OnPropertyChanged();
        }
    }

    private SubjectListModel _selectedSubjectListModel;
    public SubjectListModel SelectedSubjectListModel
    {
        get => _selectedSubjectListModel;
        set
        {
            SetProperty(ref _selectedSubjectListModel, value);
            OnPropertyChanged();
        }
    }

    public ObservableCollection<SubjectListModel> AvailableSubjects { get; set; } = new();

    [RelayCommand]
    private async Task SaveSubjectAsync()
    {
        IsAddButtonVisible = true;
        IsPickerVisible = false;
        IsSaveButtonVisible = false;

        if (_selectedSubjectListModel != null)
        {
            studentFacade.SaveSubjectStudentLink(new StudentSubjectsListModel()
            {
                Id = Guid.NewGuid(),
                StudentId = Student.Id,
                SubjectId = SelectedSubjectListModel.Id
            });
            await LoadDataAsync();
        }
    }

    [RelayCommand]
    private void AddSubject()
    {
        IsAddButtonVisible = false;
        IsPickerVisible = true;
        IsSaveButtonVisible = true;
    }

    [RelayCommand]
    private async Task RemoveAsync(Guid id)
    {
        await studentFacade.DeleteSubjectStudentLink(id, Id);
        await LoadDataAsync();
    }

    public event PropertyChangedEventHandler? PropertyChanged;

    protected virtual void OnPropertyChanged([CallerMemberName] string? propertyName = null)
    {
        PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
    }
}