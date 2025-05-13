using StudIS.BL.Facades;
using StudIS.BL.Facades.Interfaces;
using StudIS.BL.Models;
using StudIS.Common.Tests;
using StudIS.Common.Tests.Seeds;
using StudIS.DAL.Entities;

namespace StudIS.BL.Tests;

public class StudentFacadeTest : FacadeTestBase
{
    private readonly IStudentFacade _studentFacadeSUT;

    public StudentFacadeTest()
    {
        _studentFacadeSUT = new StudentFacade(UnitOfWorkFactory, StudentModelMapper);
    }
    
    [Fact]
    public async Task CreateEntity()
    {
        var studentToCreate = StudentModelMapper.MapToDetailModel(StudentSeeds.StandardInDbStudent1);
        
        await _studentFacadeSUT.SaveAsync(studentToCreate);

        Assert.NotNull(studentToCreate);
        Assert.Equal("Nikola", studentToCreate.FirstName);
    }

    
    [Fact]
    public async Task DeleteEntity()
    {
        
        await _studentFacadeSUT.DeleteAsync(StudentSeeds.StandardInDbStudent1.Id);

        var actualStudent = await _studentFacadeSUT.GetAsync(StudentSeeds.StandardInDbStudent1.Id);
        Assert.Null(actualStudent);
    }

    
    [Fact]
    public async Task UpdateGetEntity()
    {
        var existingStudent = StudentModelMapper.MapToDetailModel(StudentSeeds.StandardInDbStudent1);
        Assert.NotNull(existingStudent);
        existingStudent.FirstName = "Updated Daniil";
        
        await _studentFacadeSUT.SaveAsync(existingStudent);
        var updatedStudent = await _studentFacadeSUT.GetAsync(existingStudent.Id);
        
        Assert.Equal("Updated Daniil", updatedStudent.FirstName);
        Assert.Equal(StudentSeeds.StandardInDbStudent1.ImageUrl, updatedStudent.ImageUrl);
    }
    
    [Fact]
    public async Task DeleteNonExistingEntity()
    {
        StudentDetailModel s = StudentModelMapper.MapToDetailModel(StudentSeeds.StandardInDbStudent1);

        await _studentFacadeSUT.SaveAsync(s);
        await _studentFacadeSUT.DeleteAsync(s.Id);
        StudentDetailModel actualStudent = await _studentFacadeSUT.GetAsync(s.Id);
        Assert.Null(actualStudent);

    }
    
    [Fact]
    public async Task GetByNonExistingIdEntity()
    {
        Guid Id = Guid.Empty;
        
        StudentDetailModel actualStudent = await _studentFacadeSUT.GetAsync(Id);
        Assert.Null(actualStudent);
    }
}