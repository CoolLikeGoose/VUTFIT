using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using System.Threading.Tasks;
using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using StudIS.BL.Facades;
using StudIS.BL.Facades.Interfaces;
using StudIS.BL.Mappers;
using StudIS.BL.Models;

namespace StudIS.APP.ViewModels.Activity
{
    public partial class ActivityDetailViewModel : IViewModel, IQueryAttributable, INotifyPropertyChanged
    {
        private readonly IActivityFacade _activityFacade;
        private readonly IEvaluationFacade _evaluationFacade;
        private readonly IStudentFacade _studentFacade;
        private readonly ISubjectFacade _subjectFacade;
        private Guid Id { get; set; }
        public ActivityDetailModel? Activity { get; set; }
        public ObservableCollection<EvaluationListModel> Evaluations { get; } = new();
        public string SubjectName { get; set; } = " "; // Collection to store subject names
        
        public ActivityDetailViewModel(IActivityFacade activityFacade, IEvaluationFacade evaluationFacade, IStudentFacade studentFacade, ISubjectFacade subjectFacade)
        {
            _activityFacade = activityFacade;
            _evaluationFacade = evaluationFacade;
            _subjectFacade = subjectFacade; // Initialize the subject facade
            _studentFacade = studentFacade;
        }

        public async Task LoadDataAsync()
        {
            Activity = await _activityFacade.GetAsync(Id);
            OnPropertyChanged(nameof(Activity));

            IEnumerable<EvaluationListModel> evaluations = await _evaluationFacade.GetAsync();
            Evaluations.Clear();
            foreach (EvaluationListModel evaluation in evaluations)
            {
                var test = await _studentFacade.GetAsync(evaluation.StudentId);
                evaluation.Student = new StudentModelMapper().MapToListModel(test);
                Evaluations.Add(evaluation);
            }

            var subject = await _subjectFacade.GetAsync(Activity.SubjectId);
            SubjectName = subject.Name;
            OnPropertyChanged(nameof(SubjectName));
        }

        public async void ApplyQueryAttributes(IDictionary<string, object> query)
        {
            Id = (Guid)query["Id"];
            await LoadDataAsync();
        }

        [RelayCommand]
        private async Task DeleteAsync()
        {
            await _activityFacade.DeleteAsync(Id);
            await Shell.Current.GoToAsync("..");
        }

        [RelayCommand]
        private async Task EditAsync()
        {
            await Shell.Current.GoToAsync("edit", new Dictionary<string, object> { { "Id", Id } });
        }

        public event PropertyChangedEventHandler? PropertyChanged;

        protected virtual void OnPropertyChanged([CallerMemberName] string? propertyName = null)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }

        [RelayCommand]
        private async Task EditEvaluationAsync(Guid evaluationId)
        {
            await Shell.Current.GoToAsync("//evaluations/edit", new Dictionary<string, object> { { "Id", evaluationId }});
        }

        [RelayCommand]
        private async Task DeleteEvaluationAsync(Guid id)
        {
            await _evaluationFacade.DeleteAsync(id);
            await LoadDataAsync();
        }

        [RelayCommand]
        private async Task AddEvaluationAsync()
        {
            await Shell.Current.GoToAsync("//evaluations/edit", new Dictionary<string, object> { { "activityId", Id } });
        }
    }
}
