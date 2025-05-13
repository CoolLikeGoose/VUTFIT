using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using StudIS.BL.Facades;
using StudIS.BL.Facades.Interfaces;
using StudIS.BL.Models;
using StudIS.Common.Enums;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace StudIS.APP.ViewModels.Evaluation
{
    public partial class EvaluationEditViewModel : ObservableObject, IViewModel, IQueryAttributable
    {
        private readonly IEvaluationFacade _evaluationFacade;
        private readonly IStudentFacade _studentFacade;
        private Guid _evaluationId;
        private Guid _activityId;
        public List<StudentListModel> StudentsForChoice { get; set; }= new ();
        private StudentListModel _selectedStudentListModel;
        public StudentListModel SelectedStudentListModel
        {
            get => _selectedStudentListModel;
            set
            {
                SetProperty(ref _selectedStudentListModel, value);
                OnPropertyChanged();
            }
        }
        public EvaluationEditViewModel(IEvaluationFacade evaluationFacade, IStudentFacade studentFacade)
        {
            _evaluationFacade = evaluationFacade;
            _studentFacade = studentFacade;
        }

        private EvaluationDetailModel _evaluation;
        public EvaluationDetailModel Evaluation
        {
            get => _evaluation;
            set => SetProperty(ref _evaluation, value);
        }

        public async Task LoadDataAsync()
        {
            if (_evaluationId != Guid.Empty)
            {
                Evaluation = await _evaluationFacade.GetAsync(_evaluationId);
            }
            else
            {
                IsStudentsVisible = true;
                StudentsForChoice = (await _studentFacade.GetAsync()).ToList();
                OnPropertyChanged("StudentsForChoice");
                Evaluation = new EvaluationDetailModel
                {
                    Description = "Default Description",
                    Grade = 0,
                    ActivityId = _activityId,
                    StudentId = Guid.Empty
                };
            }
        }
        
        private bool _isStudentsVisible;
        public bool IsStudentsVisible
        {
            get => _isStudentsVisible;
            set
            {
                SetProperty(ref _isStudentsVisible, value);
                OnPropertyChanged();
            }
        }

        public async void ApplyQueryAttributes(IDictionary<string, object> query)
        {
            if (query.ContainsKey("Id"))
            {
                _evaluationId = (Guid)query["Id"];
            }
            if (query.ContainsKey("activityId"))
            {
                _activityId = (Guid)query["activityId"];
            }
            await LoadDataAsync();
        }

        [RelayCommand]
        private async Task SaveAsync()
        {
            if (IsStudentsVisible)
            {
                if (SelectedStudentListModel == null)
                {
                    return;
                }
                Evaluation.StudentId = SelectedStudentListModel.Id;
            }
            await _evaluationFacade.SaveAsync(Evaluation);
            await Shell.Current.GoToAsync("..");
        }

        [RelayCommand]
        private async Task CancelAsync()
        {
            await Shell.Current.GoToAsync("..");
        }
    }
}
