using Microsoft.EntityFrameworkCore;
using StudIS.BL.Facades.Interfaces;
using StudIS.BL.Mappers;
using StudIS.BL.Models;
using StudIS.DAL.Entities;
using StudIS.DAL.Mappers;
using StudIS.DAL.UnitOfWork;

namespace StudIS.BL.Facades;

public class SubjectFacade(IUnitOfWorkFactory unitOfWorkFactory, SubjectModelMapper subjectModelMapper) 
    : FacadeBase<SubjectEntity, SubjectListModel, SubjectDetailModel, SubjectEntityMapper>
        (unitOfWorkFactory, subjectModelMapper), ISubjectFacade
{
    public async Task<IEnumerable<SubjectListModel>> GetByName(string subjectName)
    {
        await using IUnitOfWork unitOfWork = UnitOfWorkFactory.Create();
        List<SubjectEntity> entities = await unitOfWork
            .GetRepository<SubjectEntity, SubjectEntityMapper>()
            .Get()
            .Where(e => e.Name.Contains(subjectName))
            .ToListAsync()
            .ConfigureAwait(false);

        return ModelMapper.MapToListModel(entities);
    }

    public override async Task<SubjectDetailModel?> GetAsync(Guid id)
    {
        await using IUnitOfWork unitOfWork = UnitOfWorkFactory.Create();

        SubjectEntity? entity = await unitOfWork
            .GetRepository<SubjectEntity, SubjectEntityMapper>()
            .Get()
            .Include(e => e.Activities)
            .SingleOrDefaultAsync(e => e.Id == id)
            .ConfigureAwait(false);

        return entity is null ? null : ModelMapper.MapToDetailModel(entity);
    }
}