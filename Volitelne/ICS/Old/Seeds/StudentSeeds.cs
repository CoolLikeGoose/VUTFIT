using System;
using System.Collections.Generic;
using Microsoft.EntityFrameworkCore;
using StudIS.DAL.Entities;

namespace StudIS.DAL.Seeds;
public static class StudentSeeds
{
    //TODO: update this later
    public static readonly StudentEntity BasicStudent = new StudentEntity()
    {
        Id = Guid.Parse("18b4e5b0-098f-4daa-889e-d0616d97cddf"),
        Name = "Daniil",
        ImageUrl = "https://examplepicture.example"
    };
    
    // public static readonly StudentEntity BasicStudent = new(
    //     Guid.Parse("307f3414-bd7f-430d-888e-24d9419394ee"),
    //     "A",
    //     "https://examplepicturdsade.example");
    //
    // public static readonly StudentEntity AddedStudent = new(
    //     Guid.Parse("18b4e5b0-098f-4daa-889e-d0616d97cddf"),
    //     "Daniil",
    //     "https://examplepicture.example");
    // {
    //     
    // };
    //
    // public static readonly StudentEntity OneSubjectStudent = new(
    //     Guid.Parse("7c58da0c-fecc-4a26-b541-b07e9310c6b2"),
    //     "Nikita",
    //     "https://encrypted-tbn3.gstatic.com/images?q=tbn:ANd9GcQ8QqJaYvO9Lj7t81qs89f4RZgEdoX2Ae3hfFcuWt2N-amqfrHy99Zt75yZ96ti0-cXz2P2pNQ-vWJ1WinQXQfhQWafihWM_x62j1r-TA")
    // {
    //     Subjects = new List<SubjectEntity>(){SubjectSeeds.NoActivitySubject}
    // };
    //
    // public static readonly StudentEntity MultiipleSubjectsStudent = new(
    //     Guid.Parse("8cb4c7e9-96c6-4460-a86d-c89d636c151c"),
    //     "Nikola",
    //     "https://encrypted-tbn3.gstatic.com/images?q=tbn:ANd9GcRJ6IDtr0L7Zu6ESyMN11axgYybXs2GkM2ISA7jdL4iOqDbNIn0")
    // {
    //     Subjects = new List<SubjectEntity>() { SubjectSeeds.NoActivitySubject, SubjectSeeds.OneActivitySubject, 
    //                         SubjectSeeds.MultipleActivitySubject, SubjectSeeds.MultipleActivityMultipleStudentSubject
    //     }
    // };
    //
    // public static readonly StudentEntity OtherMultipleSubjectsStudent = new(
    //     Guid.Parse("7966f0dc-d63f-4d88-9a57-c9cc52eda8fe"),
    //     "Andrey",
    //     "https://examplepicture.example")
    // {
    //     Subjects = new List<SubjectEntity>(){SubjectSeeds.MultipleActivityMultipleStudentSubject}
    // };
    public static void Seed(this ModelBuilder modelBuilder)
    {
        // modelBuilder.Entity<StudentEntity>().HasData(
        //     NoSubjectStudent,
        //     OneSubjectStudent,
        //     MultiipleSubjectsStudent,
        //     OtherMultipleSubjectsStudent
        // );
    }
}