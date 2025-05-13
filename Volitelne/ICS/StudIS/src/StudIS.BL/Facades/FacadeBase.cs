using Microsoft.EntityFrameworkCore;
using StudIS.BL.Facades.Interfaces;
using StudIS.BL.Mappers;
using StudIS.BL.Models;
using StudIS.DAL.Entities;
using StudIS.DAL.Mappers;
using StudIS.DAL.Repositories;
using StudIS.DAL.UnitOfWork;

namespace StudIS.BL.Facades;

public abstract class FacadeBase<TEntity, TListModel, TDetailModel, TEntityMapper>(
    IUnitOfWorkFactory unitOfWorkFactory,
    IModelMapper<TEntity, TListModel, TDetailModel> modelMapper) : IFacade<TEntity, TListModel, TDetailModel>
    where TEntity : class, IEntity
    where TListModel : IModel
    where TDetailModel : class, IModel
    where TEntityMapper : IEntityMapper<TEntity>, new()
{
    protected readonly IModelMapper<TEntity, TListModel, TDetailModel> ModelMapper = modelMapper;
    protected readonly IUnitOfWorkFactory UnitOfWorkFactory = unitOfWorkFactory;
    
    public async Task DeleteAsync(Guid id)
    {
        await using IUnitOfWork unitOfWork = UnitOfWorkFactory.Create();
        try
        {
            await unitOfWork.GetRepository<TEntity, TEntityMapper>().DeleteAsync(id).ConfigureAwait(false);
            await unitOfWork.CommitAsync().ConfigureAwait(false);
        }
        catch (DbUpdateException e)
        {
            throw new InvalidOperationException("Cannot delete entity", e);
        }
    }

    public virtual async Task<TDetailModel?> GetAsync(Guid id)
    {
        await using IUnitOfWork unitOfWork = UnitOfWorkFactory.Create();

        TEntity? entity = await unitOfWork
            .GetRepository<TEntity, TEntityMapper>()
            .Get()
            .SingleOrDefaultAsync(e => e.Id == id)
            .ConfigureAwait(false);

        return entity is null ? null : ModelMapper.MapToDetailModel(entity);
    }

    public async Task<IEnumerable<TListModel>> GetAsync()
    {
        await using IUnitOfWork unitOfWork = UnitOfWorkFactory.Create();
        List<TEntity> entities = await unitOfWork
            .GetRepository<TEntity, TEntityMapper>()
            .Get()
            .ToListAsync()
            .ConfigureAwait(false);
        
        return ModelMapper.MapToListModel(entities);
    }

    public async Task<TDetailModel> SaveAsync(TDetailModel model)
    {
        TDetailModel result;
        
        await using IUnitOfWork unitOfWork = unitOfWorkFactory.Create();

        TEntity entity = ModelMapper.MapToEntity(model);
        IRepository<TEntity> repository = unitOfWork.GetRepository<TEntity, TEntityMapper>();

        //if already in db
        if (await repository.ExistsAsync(entity).ConfigureAwait(false))
        {
            TEntity updEntity = await repository.UpdateAsync(entity).ConfigureAwait(false);
            result = ModelMapper.MapToDetailModel(updEntity);
        }
        else
        {
            TEntity insEntity = repository.Insert(entity);
            result = ModelMapper.MapToDetailModel(insEntity);
        }

        await unitOfWork.CommitAsync().ConfigureAwait(false);

        return result;
    }
}