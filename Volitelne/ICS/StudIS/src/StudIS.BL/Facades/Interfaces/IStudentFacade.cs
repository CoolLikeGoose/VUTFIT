using StudIS.BL.Models;
using StudIS.DAL.Entities;

namespace StudIS.BL.Facades.Interfaces;

public interface IStudentFacade : IFacade<StudentEntity, StudentListModel, StudentDetailModel>
{
    public Task<IEnumerable<StudentListModel>> GetByName(string name);
    public Task DeleteSubjectStudentLink(Guid subjectId, Guid studentId);
    public Task<StudentSubjectsDetailModel> SaveSubjectStudentLink(StudentSubjectsListModel model);
}