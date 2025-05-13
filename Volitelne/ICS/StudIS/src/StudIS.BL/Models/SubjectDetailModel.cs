using System.Collections.ObjectModel;
using StudIS.DAL.Entities;

namespace StudIS.BL.Models;

public record SubjectDetailModel : ModelBase
{
    public required string Name { get; set; }
    public required string Abbreviation { get; set; }
    public ObservableCollection<ActivityListModel> Activities { get; init; } = new ObservableCollection<ActivityListModel>();
    public ObservableCollection<StudentSubjectsListModel> Students { get; init; } = new ObservableCollection<StudentSubjectsListModel>();
    
    public static SubjectDetailModel Empty => new()
    {
        Id = Guid.NewGuid(),
        Name = string.Empty,
        Abbreviation = string.Empty
    };
}