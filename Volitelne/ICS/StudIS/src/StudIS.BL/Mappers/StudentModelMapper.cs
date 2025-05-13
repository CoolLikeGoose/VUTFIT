using System.Collections.ObjectModel;
using StudIS.BL.Models;
using StudIS.DAL.Entities;

namespace StudIS.BL.Mappers;

public class StudentModelMapper()
    : ModelMapperBase<StudentEntity, StudentListModel, StudentDetailModel>
{
    public override StudentListModel MapToListModel(StudentEntity? entity)
    {
        if (entity == null)
        {
            return StudentListModel.Empty;
        }

        return new StudentListModel()
        {
            Id = entity.Id,
            FirstName = entity.FirstName,
            LastName = entity.LastName
        };
    }
    public StudentListModel MapToListModel(StudentDetailModel? entity)
    {
        if (entity == null)
        {
            return StudentListModel.Empty;
        }

        return new StudentListModel()
        {
            Id = entity.Id,
            FirstName = entity.FirstName,
            LastName = entity.LastName
        };
    }

    public override StudentDetailModel MapToDetailModel(StudentEntity? entity)
    {
        if (entity == null)
        {
            return StudentDetailModel.Empty;
        }

        StudentSubjectsModelMapper studentSubjectsModelMapper = new StudentSubjectsModelMapper();
        return new StudentDetailModel()
        {
            Id = entity.Id,
            ImageUrl = entity.ImageUrl,
            FirstName = entity.FirstName,
            LastName = entity.LastName,
            Subjects = new ObservableCollection<StudentSubjectsListModel>
                (entity.Subjects.Select(e => studentSubjectsModelMapper.MapToListModel(e)).ToList())
        };
    }

    public override StudentEntity MapToEntity(StudentDetailModel model)
    {
        return new StudentEntity()
        {
            Id = model.Id,
            ImageUrl = model.ImageUrl,
            FirstName = model.FirstName,
            LastName = model.LastName
        };
    }
}