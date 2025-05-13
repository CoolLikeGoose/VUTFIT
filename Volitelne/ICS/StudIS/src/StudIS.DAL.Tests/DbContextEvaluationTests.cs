using Microsoft.EntityFrameworkCore;
using StudIS.Common.Tests.Seeds; 
using StudIS.DAL.Entities;
using Xunit.Abstractions;

namespace StudIS.DAL.Tests;

public class DbContextEvaluationTests(ITestOutputHelper output) : DbContextTestsBase(output)
{
    [Fact]
    public async Task AddOne_Evaluation()
    {
        EvaluationEntity evaluation = EvaluationSeeds.StandardInDbEvaluation1;
        await StudIsDbContextSUT.SaveChangesAsync();
        
        await using StudIsDbContext dbContext = await DbContextFactory.CreateDbContextAsync();
        EvaluationEntity actualEvaluation= await dbContext.Evaluations.SingleAsync(i => i.Id == evaluation.Id);
        Assert.Equal(evaluation.Id,actualEvaluation.Id);
        Assert.Equal(evaluation.Description,actualEvaluation.Description);
        Assert.Equal(evaluation.ActivityId,actualEvaluation.ActivityId);
        Assert.Equal(evaluation.StudentId,actualEvaluation.StudentId);
    }
    
     [Fact]
     public async Task Read_Evaluation_With_Related_Entities()
     {
         // Act
         await using var dbContext = await DbContextFactory.CreateDbContextAsync();
         var actualEvaluation = await dbContext.Evaluations
             .Include(e => e.Activity)
             .Include(e => e.Student)
             .SingleOrDefaultAsync(i => i.Id == EvaluationSeeds.StandardInDbEvaluation1.Id); 
    
         // Assert
         //Assert.NotNull(actualEvaluation);
         // Assert.Equal(evaluation.Description, actualEvaluation.Description);
         Assert.NotNull(actualEvaluation.Activity);
         Assert.Equal(actualEvaluation.ActivityId, EvaluationSeeds.StandardInDbEvaluation1.ActivityId);
         Assert.NotNull(actualEvaluation.Student);
         Assert.Equal(actualEvaluation.StudentId, EvaluationSeeds.StandardInDbEvaluation1.StudentId);
     }
    [Fact]
    public async Task Update_Evaluation()
    {
        // Arrange
        var evaluation = EvaluationSeeds.StandardInDbEvaluation1;

        // Act
        var updatedDescription = "Updated Evaluation Description";
        evaluation.Description = updatedDescription;
        StudIsDbContextSUT.Evaluations.Update(evaluation);
        await StudIsDbContextSUT.SaveChangesAsync();

        // Assert
        await using var dbContext = await DbContextFactory.CreateDbContextAsync();
        var updatedEvaluation = await dbContext.Evaluations.FindAsync(evaluation.Id);
        Assert.NotNull(updatedEvaluation);
        Assert.Equal(updatedDescription, updatedEvaluation.Description);
    }
    
    [Fact]
    public async Task Delete_Evaluation()
    {
        // Act
        StudIsDbContextSUT.Evaluations.Remove(EvaluationSeeds.StandardInDbEvaluation2);
        await StudIsDbContextSUT.SaveChangesAsync();
    
        // Assert
        await using var dbContext = await DbContextFactory.CreateDbContextAsync();
        var deletedEvaluation = await dbContext.Evaluations.FindAsync(EvaluationSeeds.StandardInDbEvaluation2.Id);
        Assert.Null(deletedEvaluation);
    }
}