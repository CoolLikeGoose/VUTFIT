using CommunityToolkit.Mvvm.Input;
using StudIS.BL.Facades.Interfaces;
using StudIS.BL.Models;
using System.Collections.ObjectModel;
using CommunityToolkit.Mvvm.ComponentModel;

namespace StudIS.APP.ViewModels.Student
{
    public partial class StudentListViewModel : ObservableObject, IViewModel
    {
        public ObservableCollection<StudentListModel> Students { get; } = new();
        
        private string _searchQuery;
        public string SearchQuery
        {
            get => _searchQuery;
            set => SetProperty(ref _searchQuery, value);
        }
        
        private string _sortOption;
        private readonly IStudentFacade _studentFacade;

        public string SortOption
        {
            get => _sortOption;
            set => SetProperty(ref _sortOption, value);
        }
        
        public ObservableCollection<string> SortOptions { get; } = new()
        {
            "First Name (A-Z)",
            "First Name (Z-A)",
            "Last Name (A-Z)",
            "Last Name (Z-A)"
        };
        
        public StudentListViewModel(IStudentFacade studentFacade)
        {
            _studentFacade = studentFacade;
            SortOption = SortOptions.First();
        }

        public async Task LoadDataAsync()
        {
            IEnumerable<StudentListModel> students = await _studentFacade.GetAsync();
            Students.Clear();
            foreach (StudentListModel student in students)
            {
                Students.Add(student);
            }
        }

        [RelayCommand]
        private async Task GoToCreateAsync()
        {
            await Shell.Current.GoToAsync("edit");
        }

        [RelayCommand]
        public async Task GoToDetailAsync(Guid id)
        {
            await Shell.Current.GoToAsync("detail", new Dictionary<string, object>(){{"Id", id}});
        }

        [RelayCommand]
        public async Task SearchAsync()
        {
            IEnumerable<StudentListModel> students = await _studentFacade.GetByName(SearchQuery.Trim().ToLower());
            Students.Clear();
            foreach (StudentListModel student in students)
            {
                Students.Add(student);
            }
        }

        [RelayCommand]
        public async Task RefreshAsync()
        {
            await LoadDataAsync();
        }

        [RelayCommand]
        public async Task SortAsync()
        {
            IEnumerable<StudentListModel> sortedStudents;
            
            if (SortOption == "First Name (A-Z)")
            {
                sortedStudents = Students.OrderBy(s => s.FirstName).ToList();
            }
            else if (SortOption == "First Name (Z-A)")
            {
                sortedStudents = Students.OrderByDescending(s => s.FirstName).ToList();
            }
            else if (SortOption == "Last Name (A-Z)")
            {
                sortedStudents = Students.OrderBy(s => s.LastName).ToList();
            }
            else if (SortOption == "Last Name (Z-A)")
            {
                sortedStudents = Students.OrderByDescending(s => s.LastName).ToList();
            }
            else
            {
                sortedStudents = Students.ToList();
            }

            Students.Clear();
            foreach (StudentListModel student in sortedStudents)
            {
                Students.Add(student);
            }
        }
    }
}