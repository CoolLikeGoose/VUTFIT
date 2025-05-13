using System;
using System.Collections;
using System.Collections.Generic;
using StudIS.Common.Enums;

namespace StudIS.DAL.Entities;

public record ActivityEntity : IEntity
{
    public required Guid Id { get; set; }
    public required DateTime StartTime { get; set; }
    public required DateTime EndTime { get; set; }
    public required Place Room { get; set; }
    public required ActivityType ActivityType { get; set; }
    public required string? Description { get; set; }
    public required Guid SubjectId { get; set; }
    
    public SubjectEntity? Subject { get; init; }
    public ICollection<EvaluationEntity> Evaluations { get; init; } = new List<EvaluationEntity>();
}