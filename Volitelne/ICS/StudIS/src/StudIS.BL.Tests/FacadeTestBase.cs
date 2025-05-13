using Microsoft.EntityFrameworkCore;
using StudIS.DAL;
using StudIS.BL.Mappers;
using StudIS.Common.Tests.Factories;
using StudIS.DAL.UnitOfWork;

namespace StudIS.BL.Tests;

public class FacadeTestBase : IAsyncLifetime
{
    protected IDbContextFactory<StudIsDbContext> DbContextFactory { get; }
    protected UnitOfWorkFactory UnitOfWorkFactory { get; }
    
    protected ActivityModelMapper ActivityModelMapper { get; }
    protected EvaluationModelMapper EvaluationModelMapper { get; }
    protected StudentModelMapper StudentModelMapper { get; }
    protected SubjectModelMapper SubjectModelMapper { get; }
    
    protected FacadeTestBase()
    {
        DbContextFactory = new DbContextSqLiteTestingFactory(GetType().FullName!, seedTestingData: true);
        UnitOfWorkFactory = new UnitOfWorkFactory(DbContextFactory);

        EvaluationModelMapper = new EvaluationModelMapper();
        StudentModelMapper = new StudentModelMapper();
        ActivityModelMapper = new ActivityModelMapper();
        SubjectModelMapper = new SubjectModelMapper();
    }
    
    public async Task InitializeAsync()
    {
        await using DbContext dbContext = await DbContextFactory.CreateDbContextAsync();
        await dbContext.Database.EnsureDeletedAsync();
        await dbContext.Database.EnsureCreatedAsync();
    }

    public async Task DisposeAsync()
    {
        await using DbContext dbContext = await DbContextFactory.CreateDbContextAsync();
        await dbContext.Database.EnsureDeletedAsync();
    }
}