using Microsoft.EntityFrameworkCore;
using StudIS.DAL.Entities;
using StudIS.Common.Enums;
using Xunit.Sdk;

namespace StudIS.Common.Tests.Seeds;

public static class ActivitySeeds
{
    public static readonly ActivityEntity StandardInDbActivity1 = new ActivityEntity()
    {
        Id = Guid.Parse("39c32acc-2d69-4be0-a412-44f0a738e84b"),
        StartTime = new DateTime(2023, 10, 10, 10, 10, 10),
        EndTime = new DateTime(2023, 11, 11, 11, 11, 11),
        Room = Place.D105,
        ActivityType = ActivityType.Exam,
        Description = "example description",
        SubjectId = SubjectSeeds.StandardInDbSubject1.Id,
        
        Subject = SubjectSeeds.StandardInDbSubject1
    };
    
    public static readonly ActivityEntity StandardInDbActivity2 = new ActivityEntity()
    {
        Id = Guid.Parse("0b5bb150-3fb7-4e8d-bb48-945c0c376700"),
        StartTime = new DateTime(2023, 10, 10, 10, 10, 10),
        EndTime = new DateTime(2023, 11, 11, 11, 11, 11),
        Room = Place.D105,
        ActivityType = ActivityType.Exam,
        Description = "EXAM description",
        SubjectId = SubjectSeeds.StandardInDbSubject2.Id,
        
        Subject = SubjectSeeds.StandardInDbSubject2
    };
    
    public static readonly ActivityEntity StandardInDbActivity3 = new ActivityEntity()
    {
        Id = Guid.Parse("a8e31b76-150e-496c-9579-718b9a5be9f3"),
        StartTime = new DateTime(2023, 10, 10, 10, 10, 10),
        EndTime = new DateTime(2023, 11, 11, 11, 11, 11),
        Room = Place.Laboratory,
        ActivityType = ActivityType.Exercise,
        Description = "Lab description",
        SubjectId = SubjectSeeds.StandardInDbSubject1.Id,
        
        Subject = SubjectSeeds.StandardInDbSubject1
    };
    
    public static void Seed(this ModelBuilder modelBuilder)
    {
        modelBuilder.Entity<ActivityEntity>().HasData(
            StandardInDbActivity1 with { Subject = null, Evaluations = null!},
            StandardInDbActivity2 with { Subject = null, Evaluations = null!},
            StandardInDbActivity3 with { Subject = null, Evaluations = null!}
        );
    }
}