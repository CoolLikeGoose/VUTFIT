using Microsoft.EntityFrameworkCore;
using StudIS.Common.Enums;
using StudIS.DAL.Entities;
using StudIS.DAL.Seeds;
using Xunit.Abstractions;

namespace StudIS.DAL.Tests;

public class DbContextActivityTests(ITestOutputHelper output) : DbContextTestsBase(output)
{
    [Fact]
    public async Task AddOne_Activity()
    {   
        ActivityEntity activity = ActivitySeeds.StandardInDbActivity1;
        await StudIsDbContextSUT.SaveChangesAsync();
        
        await using StudIsDbContext dbContext = await DbContextFactory.CreateDbContextAsync();
        ActivityEntity actualActivity= await dbContext.Activities.SingleAsync(i => i.Id == activity.Id);
        Assert.Equal(activity.StartTime, actualActivity.StartTime);
        Assert.Equal(activity.EndTime,actualActivity.EndTime);
        Assert.Equal(activity.Room,actualActivity.Room);
        Assert.Equal(activity.ActivityType,actualActivity.ActivityType);
    }
    [Fact]
    public async Task Read_Activity_By_Id()
    {
        // Act
        await using var dbContext = await DbContextFactory.CreateDbContextAsync();
        var actualActivity = await dbContext.Activities.FindAsync(ActivitySeeds.StandardInDbActivity1.Id);

        // Assert
        Assert.NotNull(actualActivity);
        Assert.Equal(ActivitySeeds.StandardInDbActivity1.Id, actualActivity.Id);
    }

     [Fact]
     public async Task Update_Activity()
     {
         ActivityEntity activity = ActivitySeeds.StandardInDbActivity3;
         ActivityEntity updActivity = activity with
         {
             Description = "New Description1"
         };
         
         StudIsDbContextSUT.Activities.Update(updActivity);
         await StudIsDbContextSUT.SaveChangesAsync();
     
         // Assert
         await using var dbContext = await DbContextFactory.CreateDbContextAsync();
         var actualActivity = await dbContext.Activities.FindAsync(activity.Id);
         Assert.NotNull(actualActivity);
         Assert.Equal(updActivity.Description, actualActivity.Description);
     }
     
     [Fact]
      public async Task Delete_Activity()
      {
          // Act
          StudIsDbContextSUT.Activities.Remove(ActivitySeeds.StandardInDbActivity2);
          await StudIsDbContextSUT.SaveChangesAsync();
     
          await using var dbContext = await DbContextFactory.CreateDbContextAsync();
          var deletedActivity = await dbContext.Activities.FindAsync(ActivitySeeds.StandardInDbActivity2.Id);
          Assert.Null(deletedActivity);
      }
   
     [Fact]
     public async Task Validate_Activity_Time_Interval()
     {
         // Arrange
         var invalidActivity = new ActivityEntity
         {
             Id = Guid.NewGuid(),
             StartTime = DateTime.Now,
             EndTime = DateTime.Now.AddHours(-1),
             Room = Place.D105,
             ActivityType = ActivityType.Exam,
             Description = "A test activity with invalid time interval.",
             SubjectId = Guid.NewGuid()
         };
    
         // Act & Assert
         await Assert.ThrowsAsync<DbUpdateException>(() => 
         {
             StudIsDbContextSUT.Activities.Add(invalidActivity);
             return StudIsDbContextSUT.SaveChangesAsync();
         });
         
         //Assert.IsType<DbUpdateException>(ex);
     }


}