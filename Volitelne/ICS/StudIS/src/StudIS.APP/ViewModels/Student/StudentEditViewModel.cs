using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using StudIS.BL.Facades.Interfaces;
using StudIS.BL.Models;

namespace StudIS.APP.ViewModels.Student;

public partial class StudentEditViewModel(IStudentFacade studentFacade) : ObservableObject, IViewModel, IQueryAttributable
{
    private Guid _studentId;

    private StudentDetailModel _student;

    public StudentDetailModel Student
    {
        get => _student;
        set => SetProperty(ref _student, value);
    }

    public async Task LoadDataAsync()
    {
        if (_studentId != Guid.Empty)
        {
            Student = await studentFacade.GetAsync(_studentId);
        }
        else
        {
            Student = new StudentDetailModel()
            {
                FirstName = "Default FirstName",
                LastName = "Default LastName",
                ImageUrl = ""
            };
        }
    }

    public async void ApplyQueryAttributes(IDictionary<string, object> query)
    {
        if (query.ContainsKey("Id"))
        {
            _studentId = (Guid)query["Id"];
        }
        await LoadDataAsync();
    }

    [RelayCommand]
    private async Task SaveAsync()
    {
        await studentFacade.SaveAsync(Student);
        await Shell.Current.GoToAsync("..");
    }

    [RelayCommand]
    private async Task CancelAsync()
    {
        await Shell.Current.GoToAsync("..");
    }
}