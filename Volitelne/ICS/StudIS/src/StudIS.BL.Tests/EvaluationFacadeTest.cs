using StudIS.BL.Facades;
using StudIS.BL.Facades.Interfaces;
using StudIS.BL.Models;
using StudIS.Common.Tests;
using StudIS.Common.Tests.Seeds;
using StudIS.DAL.Entities;

namespace StudIS.BL.Tests;

public class EvaluationFacadeTest : FacadeTestBase
{
    private readonly IEvaluationFacade _evaluationFacadeSUT;

    public EvaluationFacadeTest()
    {
        _evaluationFacadeSUT = new EvaluationFacade(UnitOfWorkFactory, EvaluationModelMapper);
    }
    
    [Fact]
    public async Task DeleteEvaluation()
    {
        var evaluationToDelete = EvaluationModelMapper.MapToDetailModel(EvaluationSeeds.StandardInDbEvaluation1);

        var preDeleteCheck = await _evaluationFacadeSUT.GetAsync(evaluationToDelete.Id);
        Assert.NotNull(preDeleteCheck);

        await _evaluationFacadeSUT.DeleteAsync(evaluationToDelete.Id);
        
        var postDeleteCheck = await _evaluationFacadeSUT.GetAsync(evaluationToDelete.Id);
        Assert.Null(postDeleteCheck);
    }
    
    [Fact]
    public async Task UpdateEvaluation()
    {
        var evaluation = EvaluationModelMapper.MapToDetailModel(EvaluationSeeds.StandardInDbEvaluation1);
        

        var updatedEvaluation = evaluation with
        {
            Description = "Outstanding participation"
        };

        var resultEvaluation = await _evaluationFacadeSUT.SaveAsync(updatedEvaluation);

        var actualEvaluation = await _evaluationFacadeSUT.GetAsync(evaluation.Id);
        Assert.Equal("Outstanding participation", actualEvaluation.Description);
    }
    
}
