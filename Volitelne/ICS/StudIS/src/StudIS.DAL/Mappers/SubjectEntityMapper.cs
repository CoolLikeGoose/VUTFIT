using StudIS.DAL.Entities;

namespace StudIS.DAL.Mappers;

public class SubjectEntityMapper : IEntityMapper<SubjectEntity>
{
    public void MapToExistingEntity(SubjectEntity existingEntity, SubjectEntity newEntity)
    {
        existingEntity.Abbreviation = newEntity.Abbreviation;
        existingEntity.Name = newEntity.Name;
    }
}