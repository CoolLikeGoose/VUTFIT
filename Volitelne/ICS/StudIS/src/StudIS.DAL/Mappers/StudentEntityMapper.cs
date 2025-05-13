using StudIS.DAL.Entities;

namespace StudIS.DAL.Mappers;

public class StudentEntityMapper : IEntityMapper<StudentEntity>
{
    public void MapToExistingEntity(StudentEntity existingEntity, StudentEntity newEntity)
    {
        existingEntity.FirstName = newEntity.FirstName;
        existingEntity.LastName = newEntity.LastName;
        existingEntity.ImageUrl = existingEntity.ImageUrl;
    }
}