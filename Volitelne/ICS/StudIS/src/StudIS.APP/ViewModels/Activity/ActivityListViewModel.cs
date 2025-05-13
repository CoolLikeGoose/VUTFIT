using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using StudIS.BL.Facades.Interfaces;
using StudIS.BL.Models;
using System.Collections.ObjectModel;
using System.Linq;
using System.Threading.Tasks;
using System;
using System.Collections.Generic;

namespace StudIS.APP.ViewModels.Activity
{
    public partial class ActivityListViewModel : ObservableObject, IViewModel
    {
        private readonly IActivityFacade _activityFacade;
        private readonly ISubjectFacade _subjectFacade;

        public ActivityListViewModel(IActivityFacade activityFacade, ISubjectFacade subjectFacade)
        {
            _activityFacade = activityFacade;
            _subjectFacade = subjectFacade;
            Activities = new ObservableCollection<ActivityWithSubjectName>();
            SortOptions = new ObservableCollection<string>
            {
                "Start Date (Earliest First)",
                "Start Date (Latest First)",
                "End Date (Earliest First)",
                "End Date (Latest First)",
                "Activity Type (A-Z)",
                "Activity Type (Z-A)",
                "Subject (A-Z)",
                "Subject (Z-A)"
            };
            SortOption = SortOptions.First();
        }

        public ObservableCollection<ActivityWithSubjectName> Activities { get; }

        [ObservableProperty]
        private string searchTerm;

        [ObservableProperty]
        private string sortOption;

        [ObservableProperty]
        private DateTime startDate;

        [ObservableProperty]
        private DateTime endDate;

        [ObservableProperty]
        private string selectedActivityType;

        [ObservableProperty]
        private string subject;

        public ObservableCollection<string> SortOptions { get; }

        public async Task LoadDataAsync()
        {
            IEnumerable<ActivityListModel> activities = await _activityFacade.GetAsync();
            Activities.Clear();
            foreach (ActivityListModel activity in activities)
            {
                var subject = await _subjectFacade.GetAsync(activity.SubjectId);
                Activities.Add(new ActivityWithSubjectName
                {
                    Activity = activity,
                    SubjectName = subject.Name
                });
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
            await Shell.Current.GoToAsync("detail", new Dictionary<string, object> { { "Id", id } });
        }

        [RelayCommand]
        public async Task SearchAsync()
        {
            IEnumerable<ActivityListModel> activities;

            if (string.IsNullOrWhiteSpace(SearchTerm) && StartDate == default && EndDate == default && string.IsNullOrWhiteSpace(SelectedActivityType) && string.IsNullOrWhiteSpace(Subject))
            {
                activities = await _activityFacade.GetAsync();
            }
            else
            {
                activities = await _activityFacade.FilterAsync(SearchTerm, StartDate, EndDate, SelectedActivityType, Subject);
            }

            Activities.Clear();
            foreach (ActivityListModel activity in activities)
            {
                var subject = await _subjectFacade.GetAsync(activity.SubjectId);
                Activities.Add(new ActivityWithSubjectName
                {
                    Activity = activity,
                    SubjectName = subject.Name
                });
            }
        }

        [RelayCommand]
        public void SortActivities()
        {
            var sortedActivities = Activities.ToList();
            switch (SortOption)
            {
                case "Start Date (Earliest First)":
                    sortedActivities = Activities.OrderBy(a => a.Activity.StartTime).ToList();
                    break;
                case "Start Date (Latest First)":
                    sortedActivities = Activities.OrderByDescending(a => a.Activity.StartTime).ToList();
                    break;
                case "End Date (Earliest First)":
                    sortedActivities = Activities.OrderBy(a => a.Activity.EndTime).ToList();
                    break;
                case "End Date (Latest First)":
                    sortedActivities = Activities.OrderByDescending(a => a.Activity.EndTime).ToList();
                    break;
                case "Activity Type (A-Z)":
                    sortedActivities = Activities.OrderBy(a => a.Activity.ActivityType).ToList();
                    break;
                case "Activity Type (Z-A)":
                    sortedActivities = Activities.OrderByDescending(a => a.Activity.ActivityType).ToList();
                    break;
                case "Subject (A-Z)":
                    sortedActivities = Activities.OrderBy(a => a.SubjectName).ToList();
                    break;
                case "Subject (Z-A)":
                    sortedActivities = Activities.OrderByDescending(a => a.SubjectName).ToList();
                    break;
            }
            Activities.Clear();
            foreach (var activity in sortedActivities)
            {
                Activities.Add(activity);
            }
        }

        [RelayCommand]
        public async Task RefreshAsync()
        {
            await LoadDataAsync();
        }
    }

    public class ActivityWithSubjectName
    {
        public ActivityListModel Activity { get; set; }
        public string SubjectName { get; set; }
    }
}
