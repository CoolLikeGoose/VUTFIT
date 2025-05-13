using System;
using Microsoft.EntityFrameworkCore;
using StudIS.DAL.Entities;

namespace StudIS.DAL.Seeds;

public static class EvaluationSeeds
{
    public static readonly EvaluationEntity StandardInDbEvaluation1 = new EvaluationEntity()
    {
        Id = Guid.Parse("786828c7-e163-4fa7-b83c-4a4602bb568d"),
        Grade = 50,
        Description = "description for student 1 with eval 50 act1",
        ActivityId = ActivitySeeds.StandardInDbActivity1.Id,
        StudentId = StudentSeeds.StandardInDbStudent1.Id,
        
        Activity = ActivitySeeds.StandardInDbActivity1,
        Student = StudentSeeds.StandardInDbStudent1
    };
    
    public static readonly EvaluationEntity StandardInDbEvaluation2 = new EvaluationEntity()
    {
        Id = Guid.Parse("e1259b5c-99f1-444f-8b19-1138e1a40b15"),
        Grade = 0,
        Description = "description for student2 with eval 0 act 1",
        ActivityId = ActivitySeeds.StandardInDbActivity1.Id,
        StudentId = StudentSeeds.StandardInDbStudent2.Id,
        
        Activity = ActivitySeeds.StandardInDbActivity1,
        Student = StudentSeeds.StandardInDbStudent2
    };
    
    
    public static void Seed(this ModelBuilder modelBuilder)
    {
        modelBuilder.Entity<EvaluationEntity>().HasData(
            StandardInDbEvaluation1 with { Activity = null, Student = null},
            StandardInDbEvaluation2 with { Activity = null, Student = null}
        );
    }
}