using System;
using System.Linq;
using System.Threading.Tasks;
using StudIS.DAL.Entities;

namespace StudIS.DAL.Repositories;

public interface IRepository<TEntity>
    where TEntity : class, IEntity
{
    IQueryable<TEntity> Get();
    Task DeleteAsync(Guid entityId);
    TEntity Insert(TEntity entity);
    Task<TEntity> UpdateAsync(TEntity entity);
    ValueTask<bool> ExistsAsync(TEntity entity);
}