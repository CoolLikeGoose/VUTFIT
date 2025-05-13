using Microsoft.EntityFrameworkCore;

namespace StudIS.DAL.UnitOfWork;

public class UnitOfWorkFactory(IDbContextFactory<StudIsDbContext> dbContextFactory) : IUnitOfWorkFactory
{
    public IUnitOfWork Create()
    {
        return new UnitOfWork(dbContextFactory.CreateDbContext());
    }
}