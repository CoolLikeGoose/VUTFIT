using System.Collections.ObjectModel;
using StudIS.Common.Enums;

namespace StudIS.BL.Models;

public record ActivityDetailModel : ModelBase
{
    public required DateTime StartTime { get; set; }
    public required DateTime EndTime { get; set; }
    public required Place Room { get; set; }
    public required ActivityType ActivityType { get; set; }
    public required string? Description { get; set; }
    public required Guid SubjectId { get; set; }
    
    public SubjectListModel? Subject { get; init; }
    public ObservableCollection<EvaluationListModel> Evaluations { get; init; } = new ObservableCollection<EvaluationListModel>();
    
    public static ActivityDetailModel Empty => new()
    {
        Id = Guid.NewGuid(),
        StartTime = DateTime.Now,
        EndTime = DateTime.Now,
        Room = Place.Other,
        ActivityType = ActivityType.Exercise,
        Description = string.Empty,
        SubjectId = Guid.NewGuid()
    };
}