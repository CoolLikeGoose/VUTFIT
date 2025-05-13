using Microsoft.EntityFrameworkCore;
using StudIS.DAL;
using StudIS.Common.Tests.Seeds;

namespace StudIS.Common.Tests;

public class StudIsTestingDbContext(DbContextOptions contextOptions, bool seedTestingData = false)
    : StudIsDbContext(contextOptions, seedDemoData: false)
{
    protected override void OnModelCreating(ModelBuilder modelBuilder)
    {
        base.OnModelCreating(modelBuilder);
        
        ActivitySeeds.Seed(modelBuilder);
        EvaluationSeeds.Seed(modelBuilder);
        StudentSeeds.Seed(modelBuilder);
        SubjectSeeds.Seed(modelBuilder);
        StudentsToSubjectsSeeds.Seed(modelBuilder);
    }
}