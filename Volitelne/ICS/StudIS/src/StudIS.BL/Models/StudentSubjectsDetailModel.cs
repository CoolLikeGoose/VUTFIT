namespace StudIS.BL.Models;

public record StudentSubjectsDetailModel : ModelBase
{
    public required Guid StudentId { get; set; }
    public StudentListModel? Student { get; set; }
    
    public required Guid SubjectId { get; set; }
    public SubjectListModel? Subject { get; set; }

    public static StudentSubjectsDetailModel Empty => new StudentSubjectsDetailModel()
    {
        Id = Guid.NewGuid(),
        StudentId = Guid.Empty,
        SubjectId = Guid.Empty
    };
}