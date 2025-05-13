using System;

namespace StudIS.DAL.Entities;

public record EvaluationEntity : IEntity
{
    public required Guid Id { get; set; }
    public required int Grade { get; set; }
    public required string? Description { get; set; }
    public required Guid ActivityId { get; set; }
    public required Guid StudentId { get; set; }
    
    public ActivityEntity? Activity { get; init; }
    public StudentEntity? Student { get; init; }
}