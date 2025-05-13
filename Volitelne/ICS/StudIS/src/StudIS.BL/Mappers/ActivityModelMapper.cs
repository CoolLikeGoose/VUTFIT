using System.Collections.ObjectModel;
using StudIS.BL.Models;
using StudIS.DAL.Entities;

namespace StudIS.BL.Mappers;

public class ActivityModelMapper()
    : ModelMapperBase<ActivityEntity, ActivityListModel, ActivityDetailModel>
{
    public override ActivityListModel MapToListModel(ActivityEntity? entity)
    {
        if (entity == null)
        {
            return ActivityListModel.Empty;
        }

        return new ActivityListModel()
        {
            Id = entity.Id,
            Description = entity.Description,
            StartTime = entity.StartTime,
            EndTime = entity.EndTime,
            Room = entity.Room,
            SubjectId = entity.SubjectId,
            ActivityType = entity.ActivityType
        };

    }

    public override ActivityDetailModel MapToDetailModel(ActivityEntity? entity)
    {
        if (entity == null)
        {
            return ActivityDetailModel.Empty;
        }

        EvaluationModelMapper modelMapper = new EvaluationModelMapper();
        return new ActivityDetailModel()
        {
            Id = entity.Id,
            Description = entity.Description,
            StartTime = entity.StartTime,
            EndTime = entity.EndTime,
            Room = entity.Room,
            SubjectId = entity.SubjectId,
            ActivityType = entity.ActivityType,
            Subject = new SubjectModelMapper().MapToListModel(entity.Subject),
            Evaluations = new ObservableCollection<EvaluationListModel>
                (entity.Evaluations.Select(e => modelMapper.MapToListModel(e)).ToList())
        };
    }
    
    public override ActivityEntity MapToEntity(ActivityDetailModel model)
    {
        return new ActivityEntity()
        {
            ActivityType = model.ActivityType,
            Description = model.Description,
            StartTime = model.StartTime,
            EndTime = model.EndTime,
            Id = model.Id,
            Room = model.Room,
            SubjectId = model.SubjectId,
        };
    }
}