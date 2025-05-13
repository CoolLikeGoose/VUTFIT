using System.Collections.ObjectModel;
using StudIS.BL.Models;
using StudIS.DAL.Entities;

namespace StudIS.BL.Mappers;

public class SubjectModelMapper() 
    : ModelMapperBase<SubjectEntity, SubjectListModel, SubjectDetailModel>
{
    public override SubjectListModel MapToListModel(SubjectEntity? entity)
    {
        if (entity == null)
        {
            return SubjectListModel.Empty;
        }

        return new SubjectListModel()
        {
            Id = entity.Id,
            Abbreviation = entity.Abbreviation,
            Name = entity.Name
        };
    }

    public override SubjectDetailModel MapToDetailModel(SubjectEntity? entity)
    {
        if (entity == null)
        {
            return SubjectDetailModel.Empty;
        }

        StudentSubjectsModelMapper studentSubjectsModelMapper = new StudentSubjectsModelMapper();
        ActivityModelMapper activityModelMapper = new ActivityModelMapper();
        return new SubjectDetailModel()
        {
            Id = entity.Id,
            Abbreviation = entity.Abbreviation,
            Name = entity.Name,
            Students = new ObservableCollection<StudentSubjectsListModel>
                (entity.Students.Select(e => studentSubjectsModelMapper.MapToListModel(e)).ToList()),
            Activities = new ObservableCollection<ActivityListModel>
                (entity.Activities.Select(e => activityModelMapper.MapToListModel(e)).ToList())
        };
    }

    public override SubjectEntity MapToEntity(SubjectDetailModel model)
    {
        return new SubjectEntity()
        {
            Id = model.Id,
            Abbreviation = model.Abbreviation,
            Name = model.Name
        };
    }
}