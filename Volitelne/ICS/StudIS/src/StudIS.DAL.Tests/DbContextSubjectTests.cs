using Microsoft.EntityFrameworkCore;
using StudIS.Common.Tests.Seeds; 
using StudIS.DAL.Entities;
using Xunit.Abstractions;

namespace StudIS.DAL.Tests;

public class DbContextSubjectTests(ITestOutputHelper output) : DbContextTestsBase(output)
{
    [Fact]
    public async Task AddOne_Subject()
    {
        SubjectEntity subject = SubjectSeeds.StandardInDbSubject1;
        await StudIsDbContextSUT.SaveChangesAsync();
        
        await using StudIsDbContext dbContext = await DbContextFactory.CreateDbContextAsync();
        SubjectEntity actualSubject= await dbContext.Subjects.SingleAsync(i => i.Id == subject.Id);
        Assert.Equal(subject.Name,actualSubject.Name);
        Assert.Equal(subject.Abbreviation,actualSubject.Abbreviation);
    }

    [Fact]
    public async Task Update_subject()
    {
        SubjectEntity subject = SubjectSeeds.StandardInDbSubject2;
    
        SubjectEntity subjectUpdated = subject with
        {
            Abbreviation = subject.Abbreviation + "Update"
        };

        StudIsDbContextSUT.Subjects.Update(subjectUpdated);
        await StudIsDbContextSUT.SaveChangesAsync();
        
        await using StudIsDbContext dbContext = await DbContextFactory.CreateDbContextAsync();
        SubjectEntity actualSubject = await dbContext.Subjects.SingleAsync(i => i.Id == subject.Id);
        Assert.Equal(subject.Name,actualSubject.Name);
        Assert.Equal(subjectUpdated.Abbreviation,actualSubject.Abbreviation);
    }
    [Fact]
    public async Task Read_Subject_By_Id()
    {
        // Arrange
        var subject = SubjectSeeds.StandardInDbSubject1;
        await StudIsDbContextSUT.SaveChangesAsync();

        // Act
        await using var dbContext = await DbContextFactory.CreateDbContextAsync();
        var actualSubject = await dbContext.Subjects.FindAsync(subject.Id);

        // Assert
        Assert.NotNull(actualSubject);
        Assert.Equal(subject.Name, actualSubject.Name);
        Assert.Equal(subject.Abbreviation, actualSubject.Abbreviation);
    }

    [Fact]
    public async Task Delete_Subject()
    {
        // Arrange
        StudIsDbContextSUT.Subjects.Remove(SubjectSeeds.StandardInDbSubject3);
        await StudIsDbContextSUT.SaveChangesAsync();

        // Assert
        await using var dbContext = await DbContextFactory.CreateDbContextAsync();
        var deletedSubject = await dbContext.Subjects.FindAsync(SubjectSeeds.StandardInDbSubject3.Id);
        Assert.Null(deletedSubject);
    }
}