using Microsoft.EntityFrameworkCore;
using StudIS.DAL.Entities;
using StudIS.DAL.Seeds;

namespace StudIS.DAL;

public class StudIsDbContext(DbContextOptions contextOptions, bool seedDemoData = false) : DbContext(contextOptions)
{
    public DbSet<ActivityEntity> Activities => Set<ActivityEntity>();
    public DbSet<EvaluationEntity> Evaluations => Set<EvaluationEntity>();
    public DbSet<StudentEntity> Students => Set<StudentEntity>();
    public DbSet<SubjectEntity> Subjects => Set<SubjectEntity>();
    public DbSet<StudentsToSubjectsEntity> StudentsToSubjects => Set<StudentsToSubjectsEntity>(); 
    
    protected override void OnModelCreating(ModelBuilder modelBuilder)
    {
        base.OnModelCreating(modelBuilder);
        
        //Activity
        modelBuilder.Entity<ActivityEntity>()
            .HasMany(i => i.Evaluations)
            .WithOne(i => i.Activity)
            .OnDelete(DeleteBehavior.Cascade);
        
        //Evaluation?
        
        //Subject
        modelBuilder.Entity<SubjectEntity>()
            .HasMany(i => i.Activities)
            .WithOne(i => i.Subject)
            .OnDelete(DeleteBehavior.Cascade);

        modelBuilder.Entity<SubjectEntity>()
            .HasMany(i => i.Students)
            .WithOne(i => i.Subject)
            .OnDelete(DeleteBehavior.Cascade);
        
        //Student
        modelBuilder.Entity<StudentEntity>()
            .HasMany<EvaluationEntity>()
            .WithOne(i => i.Student)
            .OnDelete(DeleteBehavior.Cascade);

        modelBuilder.Entity<StudentEntity>()
            .HasMany(i => i.Subjects)
            .WithOne(i => i.Student)
            .OnDelete(DeleteBehavior.Cascade);
        
        if (seedDemoData)
        {
            ActivitySeeds.Seed(modelBuilder);
            EvaluationSeeds.Seed(modelBuilder);
            StudentSeeds.Seed(modelBuilder);
            SubjectSeeds.Seed(modelBuilder);
            StudentsToSubjectsSeeds.Seed(modelBuilder);
        }
    }
}