using Microsoft.EntityFrameworkCore;
using StudIS.DAL;

namespace StudIS.Common.Tests.Factories;

public class DbContextSqLiteTestingFactory(string databaseName, bool seedTestingData = false) 
    : IDbContextFactory<StudIsDbContext>
{
    public StudIsDbContext CreateDbContext()
    {
        DbContextOptionsBuilder<StudIsDbContext> builder = new();
        builder.UseSqlite($"Data Source={databaseName};Cache=Shared");

        return new StudIsTestingDbContext(builder.Options, seedTestingData);
    }
}