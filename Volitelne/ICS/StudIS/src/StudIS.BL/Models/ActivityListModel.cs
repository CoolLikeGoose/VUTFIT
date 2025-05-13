using StudIS.Common.Enums;

namespace StudIS.BL.Models;

public record ActivityListModel : ModelBase
{
    public required DateTime StartTime { get; set; }
    public required DateTime EndTime { get; set; }
    public required Place Room { get; set; }
    public required string? Description { get; set; }
    
    public required ActivityType ActivityType { get; set; }
    public required Guid SubjectId { get; set; }
    public static ActivityListModel Empty => new()
    {
        Id = Guid.NewGuid(),
        StartTime = DateTime.Now,
        EndTime = DateTime.Now,
        Room = Place.Other,
        Description = string.Empty,
        SubjectId = Guid.NewGuid(),
        ActivityType = ActivityType.Exercise
    };

    
}