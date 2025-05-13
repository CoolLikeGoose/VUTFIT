using System;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.EntityFrameworkCore;
using Microsoft.EntityFrameworkCore.Metadata;
using StudIS.DAL.Entities;
using StudIS.DAL.Mappers;

namespace StudIS.DAL.Repositories;

public class Repository<TEntity>(
    DbContext dbContext,
    IEntityMapper<TEntity> entityMapper) : IRepository<TEntity> 
    where TEntity : class, IEntity
{
    private readonly DbSet<TEntity> _dbSet = dbContext.Set<TEntity>();

    public IQueryable<TEntity> Get() => _dbSet;

    public async ValueTask<bool> ExistsAsync(TEntity entity)
    {
        if (entity.Id == Guid.Empty) return false;

        return await _dbSet.AnyAsync(e => e.Id == entity.Id).ConfigureAwait(false);
    }

    public TEntity Insert(TEntity entity)
    {
        return _dbSet.Add(entity).Entity;
    }

    public async Task<TEntity> UpdateAsync(TEntity entity)
    {
        TEntity dbEntity = await _dbSet.SingleAsync(e => e.Id == entity.Id).ConfigureAwait(false);
        entityMapper.MapToExistingEntity(dbEntity, entity);
        return dbEntity;
    }
    
    public async Task DeleteAsync(Guid entityId)
    {
        _dbSet.Remove(await _dbSet.SingleAsync(e => e.Id == entityId).ConfigureAwait(false));
    }
}