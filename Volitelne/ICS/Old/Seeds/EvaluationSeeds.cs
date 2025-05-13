using System;
using Microsoft.EntityFrameworkCore;
using StudIS.DAL.Entities;

namespace StudIS.DAL.Seeds;

public static class EvaluationSeeds
{
    //TODO: update this later
    public static readonly EvaluationEntity BasicEvaluation = new EvaluationEntity()
    {
        Id = Guid.Parse("e5b05c91-b092-4064-afa5-da1aeb0e4028"),
        Description = "example description",
        ActivityId = ActivitySeeds.BasicActivity.Id,
        StudentId = StudentSeeds.BasicStudent.Id
    };
    
    public static void Seed(this ModelBuilder modelBuilder)
    {
        // modelBuilder.Entity<EvaluationEntity>().HasData(
        //     MultipleSubjectStudentEvaluation
        // );
    }
}