using StudIS.DAL.Entities;

namespace StudIS.DAL.Mappers;

public class ActivityEntityMapper : IEntityMapper<ActivityEntity>
{
    public void MapToExistingEntity(ActivityEntity existingEntity, ActivityEntity newEntity)
    {
        existingEntity.StartTime = newEntity.StartTime;
        existingEntity.EndTime = newEntity.EndTime;
        existingEntity.Description = newEntity.Description;
        existingEntity.ActivityType = newEntity.ActivityType;
        existingEntity.Room = newEntity.Room;
        existingEntity.SubjectId = newEntity.SubjectId;
    }
}