using StudIS.DAL.Entities;

namespace StudIS.BL.Models;

public record EvaluationListModel : ModelBase
{
    public required string? Description { get; set; }
    public required int Grade { get; set; }
    public required Guid ActivityId { get; set; }
    public required Guid StudentId { get; set; }
    
    public ActivityListModel? Activity { get; init; }
    public StudentListModel? Student { get; set; }
    
    public static EvaluationListModel Empty => new()
    {
        Id = Guid.NewGuid(),
        Grade = 0,
        Description = string.Empty,
        ActivityId = Guid.NewGuid(),
        StudentId = Guid.NewGuid()
    };
}