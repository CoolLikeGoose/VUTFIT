using Microsoft.EntityFrameworkCore;
using StudIS.BL.Facades;
using StudIS.BL.Facades.Interfaces;
using StudIS.BL.Models;
using StudIS.Common.Tests;
using StudIS.Common.Tests.Seeds;
using StudIS.DAL.Entities;

namespace StudIS.BL.Tests;

public class SubjectFacadeTest : FacadeTestBase
{
    private readonly ISubjectFacade _subjectFacadeSUT;
    
    public SubjectFacadeTest()
    {
        _subjectFacadeSUT = new SubjectFacade(UnitOfWorkFactory, SubjectModelMapper);
    }
    
    [Fact]
    public async Task CreateEntity()
    {
        SubjectDetailModel s = SubjectModelMapper.MapToDetailModel(SubjectSeeds.StandardInDbSubject1);

        var savedSubject  = await _subjectFacadeSUT.SaveAsync(s);

        Assert.NotNull(savedSubject);
        Assert.Equal(s.Id, savedSubject.Id);
        Assert.Equal("mathematics analysis", savedSubject.Name);
        Assert.Equal("ima1", savedSubject.Abbreviation);
    }
    
    [Fact]
    public async Task DeleteEntity()
    {
        SubjectDetailModel s = SubjectModelMapper.MapToDetailModel(SubjectSeeds.StandardInDbSubject1);

        var savedSubject = await _subjectFacadeSUT.SaveAsync(s);
        Assert.NotNull(savedSubject);
        
        await _subjectFacadeSUT.DeleteAsync(s.Id);
        var fetchedSubject = await _subjectFacadeSUT.GetAsync(s.Id);
        Assert.Null(fetchedSubject);
    }
    
    [Fact]
    public async Task UpdateGetEntity()
    {
        var initialSubject = SubjectModelMapper.MapToDetailModel(SubjectSeeds.StandardInDbSubject1);
        var updatedSubject = initialSubject with
        {
            Name = "Updated " + initialSubject.Name,
        };
        await _subjectFacadeSUT.SaveAsync(updatedSubject);
        
        var actualSubject = await _subjectFacadeSUT.GetAsync(updatedSubject.Id);
        
        Assert.Equal(updatedSubject.Name, actualSubject.Name);
        Assert.Equal(initialSubject.Abbreviation, actualSubject.Abbreviation);

    }
    
    [Fact]
    public async Task DeleteNonExistingEntity()
    {
        SubjectDetailModel s = SubjectModelMapper.MapToDetailModel(SubjectSeeds.StandardInDbSubject1);

        await _subjectFacadeSUT.SaveAsync(s);
        await _subjectFacadeSUT.DeleteAsync(s.Id);
        SubjectDetailModel actualSubject = await _subjectFacadeSUT.GetAsync(s.Id);
        Assert.Null(actualSubject);
    }
    
    [Fact]
    public async Task GetByNonExistingIdEntity()
    {
        Guid Id = Guid.Empty;
        
        SubjectDetailModel actualSubject = await _subjectFacadeSUT.GetAsync(Id);
        Assert.Null(actualSubject);
    }
}