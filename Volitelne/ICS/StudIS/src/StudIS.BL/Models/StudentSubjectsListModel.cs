namespace StudIS.BL.Models;

public record StudentSubjectsListModel : ModelBase
{
    public required Guid StudentId { get; set; }
    public StudentListModel? Student { get; set; }
    
    public required Guid SubjectId { get; set; }
    public SubjectListModel? Subject { get; set; }

    public static StudentSubjectsListModel Empty => new StudentSubjectsListModel()
    {
        Id = Guid.NewGuid(),
        StudentId = Guid.Empty,
        SubjectId = Guid.Empty
    };
}