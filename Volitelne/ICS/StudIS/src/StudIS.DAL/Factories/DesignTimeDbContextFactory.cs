using Microsoft.EntityFrameworkCore.Design;

namespace StudIS.DAL.Factories;

public class DesignTimeDbContextFactory : IDesignTimeDbContextFactory<StudIsDbContext>
{
    private readonly DbContextSqLiteFactory _dbContextSqLiteFactory = new("StudIS");

    public StudIsDbContext CreateDbContext(string[] args) => _dbContextSqLiteFactory.CreateDbContext();
}