using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using StudIS.BL.Facades.Interfaces;
using StudIS.BL.Models;
using System.Collections.ObjectModel;
using System.Diagnostics;

namespace StudIS.APP.ViewModels.Subjects
{
    public partial class SubjectsListViewModel : ObservableObject, IViewModel
    {
        private readonly ISubjectFacade _subjectFacade;

        public ObservableCollection<SubjectListModel> Subjects { get; } = new();

        private string _searchQuery;
        public string SearchQuery
        {
            get => _searchQuery;
            set => SetProperty(ref _searchQuery, value);
        }

        private string _sortOption;
        public string SortOption
        {
            get => _sortOption;
            set => SetProperty(ref _sortOption, value);
        }

        public ObservableCollection<string> SortOptions { get; } = new()
        {
            "Name (A-Z)",
            "Name (Z-A)",
            "Abbreviation (A-Z)",
            "Abbreviation (Z-A)"
        };

        public SubjectsListViewModel(ISubjectFacade subjectFacade)
        {
            _subjectFacade = subjectFacade;
            SortOption = SortOptions.First(); // Initialize SortOption with a default value
        }

        public async Task LoadDataAsync()
        {
            IEnumerable<SubjectListModel> subjects = await _subjectFacade.GetAsync();
            Subjects.Clear();
            foreach (SubjectListModel subject in subjects)
            {
                Subjects.Add(subject);
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
            await Shell.Current.GoToAsync("detail", new Dictionary<string, object>() { { "Id", id } });
        }

        [RelayCommand]
        public async Task SearchAsync()
        {
            IEnumerable<SubjectListModel> subjects = await _subjectFacade.GetByName(SearchQuery); // Use the search query
            Subjects.Clear();
            foreach (SubjectListModel subject in subjects)
            {
                Subjects.Add(subject);
            }
        }

        [RelayCommand]
        public async Task SortAsync()
        {
            IEnumerable<SubjectListModel> sortedSubjects;

            // Sort the subjects based on the selected sort option
            if (SortOption == "Name (A-Z)")
            {
                sortedSubjects = Subjects.OrderBy(s => s.Name).ToList();
            }
            else if (SortOption == "Name (Z-A)")
            {
                sortedSubjects = Subjects.OrderByDescending(s => s.Name).ToList();
            }
            else if (SortOption == "Abbreviation (A-Z)")
            {
                sortedSubjects = Subjects.OrderBy(s => s.Abbreviation).ToList();
            }
            else if (SortOption == "Abbreviation (Z-A)")
            {
                sortedSubjects = Subjects.OrderByDescending(s => s.Abbreviation).ToList();
            }
            else
            {
                sortedSubjects = Subjects.ToList(); // Default case, no sorting
            }


            // Update the Subjects collection with the sorted subjects
            Subjects.Clear();
            foreach (SubjectListModel subject in sortedSubjects)
            {
                Subjects.Add(subject);
            }
        }

        [RelayCommand]
        public async Task RefreshAsync()
        {
            await LoadDataAsync();
        }
    }
}
