using System;
using System.Threading.Tasks;
using Microsoft.EntityFrameworkCore;
using StudIS.DAL.Entities;
using StudIS.DAL.Mappers;
using StudIS.DAL.Repositories;

namespace StudIS.DAL.UnitOfWork;

public class UnitOfWork(DbContext dbContext) : IUnitOfWork
{
    private readonly DbContext _dbContext = dbContext ?? throw new ArgumentNullException(nameof(dbContext));
    
    public async ValueTask DisposeAsync()
    {
        await _dbContext.DisposeAsync().ConfigureAwait(false);
    }

    public IRepository<TEntity> GetRepository<TEntity, TEntityMapper>() 
        where TEntity : class, IEntity 
        where TEntityMapper : IEntityMapper<TEntity>, new()
    {
        return new Repository<TEntity>(_dbContext, new TEntityMapper());
    }

    public async Task CommitAsync()
    {
        await _dbContext.SaveChangesAsync().ConfigureAwait(false);
    }
}