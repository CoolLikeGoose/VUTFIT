using System;
using System.Collections.Generic;
using Microsoft.EntityFrameworkCore;
using StudIS.DAL.Entities;

namespace StudIS.DAL.Seeds;

public static class SubjectSeeds
{
    //TODO: update this later
    public static readonly SubjectEntity BasicSubject = new SubjectEntity()
    {
        Id = Guid.Parse("e5dc71e1-c526-478c-97d0-d029129d5c8d"),
        Name = "mathematics analysis",
        Abbreviation = "ima1"
    };

    public static SubjectEntity SubjectUpdateTest = new SubjectEntity()
    {
        Id = Guid.Parse("15ed62ae-6ec5-4643-9dd8-4f6e5e0b0a0e"),
        Name = "some shit",
        Abbreviation = "SHT"
    };
    // {
    //     Students = new List<StudentEntity>(){StudentSeeds.MultiipleSubjectsStudent,StudentSeeds.OneSubjectStudent}
    // };
    //
    // public static readonly SubjectEntity OneActivitySubject = new(
    //     Guid.Parse("c78d79bc-74a1-4b7d-90c3-45b833f3e6f1"),
    //     "mathematic analysis 2",
    //     "ima2"
    // )
    // {
    //     Students = new List<StudentEntity>(){StudentSeeds.MultiipleSubjectsStudent},
    //     Activities = new List<ActivityEntity>(){ActivitySeeds.NoEvaluationActivity}
    // };
    // public static readonly SubjectEntity MultipleActivitySubject = new(
    //     Guid.Parse("89848ab3-669a-469e-b7cd-60d689c26723"),
    //     "database system",
    //     "ids"
    // )
    // {
    //     Students = new List<StudentEntity>(){StudentSeeds.MultiipleSubjectsStudent},
    //     Activities = new List<ActivityEntity>(){ActivitySeeds.EvaluationActivity,ActivitySeeds.NoEvaluationActivity1}
    // };
    // public static readonly SubjectEntity MultipleActivityMultipleStudentSubject = new(
    //     Guid.Parse("62bce783-9813-45ec-b46f-01327e5a032b"),
    //     "matlab seminar",
    //     "imt"
    // )
    // {
    //     Students = new List<StudentEntity>(){StudentSeeds.MultiipleSubjectsStudent,StudentSeeds.OtherMultipleSubjectsStudent},
    //     Activities = new List<ActivityEntity>(){ActivitySeeds.NoEvaluationActivity2,ActivitySeeds.NoEvaluationActivity3}
    // };
    public static void Seed(this ModelBuilder modelBuilder)
    {
        modelBuilder.Entity<SubjectEntity>().HasData(
            SubjectUpdateTest
        );
    }
}