using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using StudIS.BL.Facades.Interfaces;
using StudIS.BL.Models;
using StudIS.Common.Enums;

namespace StudIS.APP.ViewModels.Activity
{
    public partial class ActivityEditViewModel : ObservableObject, IViewModel, IQueryAttributable
    {
        private readonly IActivityFacade _activityFacade;
        private readonly ISubjectFacade _subjectFacade;
        private Guid _activityId;

        public ActivityEditViewModel(IActivityFacade activityFacade, ISubjectFacade subjectFacade)
        {
            _activityFacade = activityFacade;
            _subjectFacade = subjectFacade;
            ActivityTypes = Enum.GetValues(typeof(ActivityType)).Cast<ActivityType>().ToList();
            Rooms = Enum.GetValues(typeof(Place)).Cast<Place>().ToList();
            Subjects = new List<SubjectListModel>();
            LoadSubjects();
        }

        private ActivityDetailModel _activity;
        public ActivityDetailModel Activity
        {
            get => _activity;
            set => SetProperty(ref _activity, value);
        }

        public List<ActivityType> ActivityTypes { get; }
        public List<Place> Rooms { get; }
        public List<SubjectListModel> Subjects { get; private set; }

        private async void LoadSubjects()
        {
            Subjects = (await _subjectFacade.GetByName(string.Empty)).ToList();
        }

        public async Task LoadDataAsync()
        {
            if (_activityId != Guid.Empty)
            {
                Activity = await _activityFacade.GetAsync(_activityId);
            }
            else
            {
                Activity = new ActivityDetailModel
                {
                    Description = "Default Description",
                    ActivityType = ActivityType.Exercise,
                    StartTime = DateTime.Now,
                    EndTime = DateTime.Now.AddHours(1),
                    Room = Place.Laboratory,
                    SubjectId = Subjects.FirstOrDefault()?.Id ?? Guid.Empty
                };
            }

            if (Activity.SubjectId == default && Subjects.Any())
            {
                Activity.SubjectId = Subjects.First().Id;
            }
        }

        public async void ApplyQueryAttributes(IDictionary<string, object> query)
        {
            if (query.ContainsKey("Id"))
            {
                _activityId = (Guid)query["Id"];
            }
            await LoadDataAsync();
        }

        [RelayCommand]
        private async Task SaveAsync()
        {
            await _activityFacade.SaveAsync(Activity);
            await Shell.Current.GoToAsync("..");
        }

        [RelayCommand]
        private async Task CancelAsync()
        {
            await Shell.Current.GoToAsync("..");
        }
    }
}
