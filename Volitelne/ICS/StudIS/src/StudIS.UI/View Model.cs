using StudIS.BL.Facades;
using StudIS.BL.Models;


public class MainViewModel
{
    private readonly StudentFacade _studentFacade;
    private readonly EvaluationFacade _evaluationFacade;
    

    public MainViewModel(StudentFacade studentFacade, EvaluationFacade evaluationFacade)
    {
        _studentFacade = studentFacade;
        _evaluationFacade = evaluationFacade;
    }

    
    public async Task <IEnumerable<StudentListModel>> GetStudents()
    {
        return await _studentFacade.GetAsync();
    }

    public async Task <IEnumerable<EvaluationListModel>> GetEvaluationsForStudent(Guid studentId)
    { 
        return await _evaluationFacade.GetByStudent(studentId);
    }
}