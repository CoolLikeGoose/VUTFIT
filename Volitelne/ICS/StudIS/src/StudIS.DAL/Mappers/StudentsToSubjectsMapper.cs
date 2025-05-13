using StudIS.DAL.Entities;

namespace StudIS.DAL.Mappers;

public class StudentsToSubjectsMapper : IEntityMapper<StudentsToSubjectsEntity>
{
    public void MapToExistingEntity(StudentsToSubjectsEntity existingEntity, StudentsToSubjectsEntity newEntity)
    {
        existingEntity.SubjectId = newEntity.SubjectId;
        existingEntity.Subject = newEntity.Subject;
        existingEntity.StudentId = newEntity.StudentId;
        existingEntity.Student = newEntity.Student;
    }
}