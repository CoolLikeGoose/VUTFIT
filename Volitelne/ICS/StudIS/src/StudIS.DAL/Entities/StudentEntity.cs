using System;
using System.Collections.Generic;

namespace StudIS.DAL.Entities;

public record StudentEntity : IEntity
{
    public required Guid Id { get; set; }
    public required string FirstName { get; set; }
    public required string LastName { get; set; }
    public required string? ImageUrl { get; set; }
    
    public ICollection<StudentsToSubjectsEntity> Subjects { get; init; } = new List<StudentsToSubjectsEntity>();
}
