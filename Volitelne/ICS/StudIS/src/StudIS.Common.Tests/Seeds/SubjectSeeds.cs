namespace StudIS.Common.Tests.Seeds;
using StudIS.DAL.Entities;
using Microsoft.EntityFrameworkCore;
public static class SubjectSeeds
{
    public static readonly SubjectEntity StandardInDbSubject1 = new SubjectEntity()
    {
        Id = Guid.Parse("5ca935c9-7abd-4e44-9004-9339c319e495"),
        Name = "mathematics analysis",
        Abbreviation = "ima1"
    };

    public static SubjectEntity StandardInDbSubject2 = new SubjectEntity()
    {
        Id = Guid.Parse("41f887c4-36f2-42f9-88c8-68b77634cfa4"),
        Name = "mathematic analysis 2",
        Abbreviation = "ima2"
    };
    
    public static SubjectEntity StandardInDbSubject3 = new SubjectEntity()
    {
        Id = Guid.Parse("1e17588a-fe30-4c77-9529-69579adc30bb"),
        Name = "database system",
        Abbreviation = "ids"
    };
    
    public static void Seed(this ModelBuilder modelBuilder)
    {
        modelBuilder.Entity<SubjectEntity>().HasData(
            StandardInDbSubject1 with { Activities = null!, Students = null! },
            StandardInDbSubject2 with { Activities = null!, Students = null! },
            StandardInDbSubject3 with { Activities = null!, Students = null! }
        );
    }
}