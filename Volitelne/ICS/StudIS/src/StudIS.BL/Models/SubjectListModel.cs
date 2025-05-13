namespace StudIS.BL.Models;

public record SubjectListModel : ModelBase
{
    public required string Name { get; set; }
    public required string Abbreviation { get; set; }
    
    public static SubjectListModel Empty => new()
    {
        Id = Guid.NewGuid(),
        Name = string.Empty,
        Abbreviation = string.Empty
    };
}