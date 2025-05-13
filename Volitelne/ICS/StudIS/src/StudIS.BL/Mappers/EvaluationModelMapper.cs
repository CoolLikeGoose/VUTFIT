using StudIS.BL.Models;
using StudIS.DAL.Entities;
using StudIS.DAL.Mappers;

namespace StudIS.BL.Mappers;

public class EvaluationModelMapper()
    : ModelMapperBase<EvaluationEntity, EvaluationListModel, EvaluationDetailModel>
{
    public override EvaluationListModel MapToListModel(EvaluationEntity? entity)
    {
        if (entity == null)
        {
            return EvaluationListModel.Empty;
        }

        return new EvaluationListModel()
        {
            Activity = new ActivityModelMapper().MapToListModel(entity.Activity),
            ActivityId = entity.ActivityId,
            Description = entity.Description,
            Grade = entity.Grade,
            Id = entity.Id,
            StudentId = entity.StudentId,
            Student = new StudentModelMapper().MapToListModel(entity.Student)
        };
    }

    public override EvaluationDetailModel MapToDetailModel(EvaluationEntity? entity)
    {
        if (entity == null)
        {
            return EvaluationDetailModel.Empty;
        }

        return new EvaluationDetailModel()
        {
            Id = entity.Id,
            Activity = new ActivityModelMapper().MapToListModel(entity.Activity),
            Grade = entity.Grade,
            ActivityId = entity.ActivityId,
            Description = entity.Description,
            Student = new StudentModelMapper().MapToListModel(entity.Student),
            StudentId = entity.StudentId
        };
    }

    public override EvaluationEntity MapToEntity(EvaluationDetailModel model)
    {
        return new EvaluationEntity()
        {
            Id = model.Id,
            ActivityId = model.ActivityId,
            Description = model.Description,
            Grade = model.Grade,
            StudentId = model.StudentId
        };
    }
}