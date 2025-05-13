using StudIS.BL.Models;
using StudIS.DAL.Entities;

namespace StudIS.BL.Facades.Interfaces;

public interface ISubjectFacade : IFacade<SubjectEntity, SubjectListModel, SubjectDetailModel>
{
    public Task<IEnumerable<SubjectListModel>> GetByName(string Name);
}