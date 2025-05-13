using System;
using Microsoft.EntityFrameworkCore;
using StudIS.DAL.Entities;

namespace StudIS.DAL.Seeds;

public static class StudentsToSubjectsSeeds
{
    public static readonly StudentsToSubjectsEntity Stud1ToSubj1 = new StudentsToSubjectsEntity()
    {
        Id = Guid.Parse("48213139-46be-4920-ab98-5a49e1978f49"),
        
        SubjectId = SubjectSeeds.StandardInDbSubject1.Id,
        StudentId = StudentSeeds.StandardInDbStudent1.Id,
        
        Subject = SubjectSeeds.StandardInDbSubject1,
        Student = StudentSeeds.StandardInDbStudent1
    };
    
    public static readonly StudentsToSubjectsEntity Stud2ToSubj1 = new StudentsToSubjectsEntity()
    {
        Id = Guid.Parse("7bde03b2-a5da-4f31-bab6-79157099d352"),
        
        SubjectId = SubjectSeeds.StandardInDbSubject1.Id,
        StudentId = StudentSeeds.StandardInDbStudent2.Id,
        
        Subject = SubjectSeeds.StandardInDbSubject1,
        Student = StudentSeeds.StandardInDbStudent2
    };
    
    public static readonly StudentsToSubjectsEntity Stud3ToSubj1 = new StudentsToSubjectsEntity()
    {
        Id = Guid.Parse("9272a2fa-effd-4d8c-a2ee-93b7c010e219"),
        
        SubjectId = SubjectSeeds.StandardInDbSubject1.Id,
        StudentId = StudentSeeds.StandardInDbStudent3.Id,
        
        Subject = SubjectSeeds.StandardInDbSubject1,
        Student = StudentSeeds.StandardInDbStudent3
    };
    
    public static readonly StudentsToSubjectsEntity Stud3ToSubj2 = new StudentsToSubjectsEntity()
    {
        Id = Guid.Parse("33933e09-a345-4ba1-906e-1a20090f06cb"),
        
        SubjectId = SubjectSeeds.StandardInDbSubject2.Id,
        StudentId = StudentSeeds.StandardInDbStudent3.Id,
        
        Subject = SubjectSeeds.StandardInDbSubject2,
        Student = StudentSeeds.StandardInDbStudent3
    };
    
    public static readonly StudentsToSubjectsEntity Stud2ToSubj2 = new StudentsToSubjectsEntity()
    {
        Id = Guid.Parse("19e2216f-9d63-4e19-bd64-fae939f84af9"),
        
        SubjectId = SubjectSeeds.StandardInDbSubject2.Id,
        StudentId = StudentSeeds.StandardInDbStudent2.Id,
        
        Subject = SubjectSeeds.StandardInDbSubject2,
        Student = StudentSeeds.StandardInDbStudent2
    };
    
    public static void Seed(this ModelBuilder modelBuilder)
    {
        modelBuilder.Entity<StudentsToSubjectsEntity>().HasData(
            Stud1ToSubj1 with { Subject = null!, Student = null!},
            Stud2ToSubj1 with { Subject = null!, Student = null!},
            Stud3ToSubj1 with { Subject = null!, Student = null!},
            Stud3ToSubj2 with { Subject = null!, Student = null!},
            Stud2ToSubj2 with { Subject = null!, Student = null!}
        );
    }
}