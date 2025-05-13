using System;
using System.Collections.Generic;

namespace StudIS.DAL.Entities;

public record SubjectEntity : IEntity
{
    public required Guid Id { get; set; }
    public required string Name { get; set; }
    public required string Abbreviation { get; set; }
    public ICollection<ActivityEntity> Activities { get; init; } = new List<ActivityEntity>();
    public ICollection<StudentsToSubjectsEntity> Students { get; init; } = new List<StudentsToSubjectsEntity>();
}