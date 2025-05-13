using StudIS.BL.Facades;
using StudIS.BL.Facades.Interfaces;
using StudIS.BL.Models;
using StudIS.Common.Enums;
using StudIS.Common.Tests;
using StudIS.Common.Tests.Seeds;
using StudIS.DAL.Entities;

namespace StudIS.BL.Tests;


public class ActivityFacadeTest : FacadeTestBase
{
    private readonly IActivityFacade _activityFacadeSUT;

    public ActivityFacadeTest()
    { 
        _activityFacadeSUT = new ActivityFacade(UnitOfWorkFactory, ActivityModelMapper);
    }
    
    [Fact]
    public async Task CreateEntity()
    {
        var activityToCreate  = ActivityModelMapper.MapToDetailModel(ActivitySeeds.StandardInDbActivity1);
        
        var savedActivity = await _activityFacadeSUT.SaveAsync(activityToCreate);
        
        Assert.NotNull(savedActivity);
        Assert.Equal(ActivitySeeds.StandardInDbActivity1.Description, savedActivity.Description);
    }
    [Fact]
    public async Task DeleteActivity()
    {
        var activityToDelete = ActivityModelMapper.MapToDetailModel(ActivitySeeds.StandardInDbActivity1);
        
        var preDeleteCheck = await _activityFacadeSUT.GetAsync(activityToDelete.Id);
        Assert.NotNull(preDeleteCheck);

        await _activityFacadeSUT.DeleteAsync(activityToDelete.Id);
        
        var postDeleteCheck = await _activityFacadeSUT.GetAsync(activityToDelete.Id);
        Assert.Null(postDeleteCheck);
    }
    [Fact]
    public async Task UpdateActivity()
    {
        var activityToUpdate = ActivityModelMapper.MapToDetailModel(ActivitySeeds.StandardInDbActivity1);

        var updatedActivity = activityToUpdate with
        {
            Description = "Updated Activity Description"
        };

        await _activityFacadeSUT.SaveAsync(updatedActivity);

        var actualActivity = await _activityFacadeSUT.GetAsync(activityToUpdate.Id);
        Assert.Equal("Updated Activity Description", actualActivity.Description);
    }
}
