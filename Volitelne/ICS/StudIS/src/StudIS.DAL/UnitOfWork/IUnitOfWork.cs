using System;
using System.Threading.Tasks;
using StudIS.DAL.Entities;
using StudIS.DAL.Mappers;
using StudIS.DAL.Repositories;

namespace StudIS.DAL.UnitOfWork;

public interface IUnitOfWork : IAsyncDisposable
{
    IRepository<TEntity> GetRepository<TEntity, TEntityMapper>()
        where TEntity : class, IEntity
        where TEntityMapper : IEntityMapper<TEntity>, new();

    Task CommitAsync();
}