using Microsoft.EntityFrameworkCore;

namespace StudIS.DAL.Factories;

public class DbContextSqLiteFactory : IDbContextFactory<StudIsDbContext>
{
    private readonly bool _seedTestingData;
    private readonly DbContextOptionsBuilder<StudIsDbContext> _contextOptionsBuilder = new();
    
    public DbContextSqLiteFactory(string databaseName, bool seedTestingData = false)
    {
        _seedTestingData = seedTestingData;
        _contextOptionsBuilder.UseSqlite($"Data Source={databaseName};Cache=Shared");
    }

    public StudIsDbContext CreateDbContext() => new(_contextOptionsBuilder.Options, _seedTestingData);
}