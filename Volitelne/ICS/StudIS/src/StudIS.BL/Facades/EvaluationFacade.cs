using Microsoft.EntityFrameworkCore;
using StudIS.BL.Facades.Interfaces;
using StudIS.BL.Mappers;
using StudIS.BL.Models;
using StudIS.DAL.Entities;
using StudIS.DAL.Mappers;
using StudIS.DAL.UnitOfWork;

namespace StudIS.BL.Facades;

public class EvaluationFacade(IUnitOfWorkFactory unitOfWorkFactory, EvaluationModelMapper evaluationModelMapper) 
    : FacadeBase<EvaluationEntity, EvaluationListModel, EvaluationDetailModel, EvaluationEntityMapper>
    (unitOfWorkFactory, evaluationModelMapper), IEvaluationFacade
{
    public async Task<IEnumerable<EvaluationListModel>> GetByStudent(Guid studentId)
    {
        await using IUnitOfWork unitOfWork = UnitOfWorkFactory.Create();
        List<EvaluationEntity> entities = await unitOfWork
            .GetRepository<EvaluationEntity, EvaluationEntityMapper>()
            .Get()
            .Where(e => e.StudentId == studentId)
            .ToListAsync()
            .ConfigureAwait(false);

        return ModelMapper.MapToListModel(entities);
    }
}