using StudIS.DAL.Entities;

namespace StudIS.DAL.Mappers;

public interface IEntityMapper<in TEntity>
    where TEntity : IEntity
{
    void MapToExistingEntity(TEntity existingEntity, TEntity newEntity);
}