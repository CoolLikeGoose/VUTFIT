using StudIS.BL.Models;
using StudIS.DAL.Entities;

namespace StudIS.BL.Mappers;

public class StudentSubjectsModelMapper
    : ModelMapperBase<StudentsToSubjectsEntity, StudentSubjectsListModel, StudentSubjectsDetailModel>
{
    public override StudentSubjectsListModel MapToListModel(StudentsToSubjectsEntity? entity)
    {
        if (entity == null)
        {
            return StudentSubjectsListModel.Empty;
        }

        StudentModelMapper studentModelMapper = new StudentModelMapper();
        SubjectModelMapper subjectModelMapper = new SubjectModelMapper();
        return new StudentSubjectsListModel()
        {
            Id = entity.Id,
            
            SubjectId = entity.SubjectId,
            StudentId = entity.StudentId,
            
            Subject = subjectModelMapper.MapToListModel(entity.Subject),
            Student = studentModelMapper.MapToListModel(entity.Student)
        };
    }

    public override StudentSubjectsDetailModel MapToDetailModel(StudentsToSubjectsEntity entity)
    {
        if (entity == null)
        {
            return StudentSubjectsDetailModel.Empty;
        }

        StudentModelMapper studentModelMapper = new StudentModelMapper();
        SubjectModelMapper subjectModelMapper = new SubjectModelMapper();
        return new StudentSubjectsDetailModel()
        {
            Id = entity.Id,
            
            SubjectId = entity.SubjectId,
            StudentId = entity.StudentId,
            
            Subject = subjectModelMapper.MapToListModel(entity.Subject),
            Student = studentModelMapper.MapToListModel(entity.Student)
        };
    }
    
    public override StudentsToSubjectsEntity MapToEntity(StudentSubjectsDetailModel model)
    {
        return new StudentsToSubjectsEntity()
        {
            Id = model.Id,

            StudentId = model.StudentId,
            SubjectId = model.SubjectId,

            Student = null!,
            Subject = null!
        };
    }
    
    public StudentsToSubjectsEntity MapToEntity(StudentSubjectsListModel model)
    {
        return new StudentsToSubjectsEntity()
        {
            Id = model.Id,

            StudentId = model.StudentId,
            SubjectId = model.SubjectId,

            Student = null!,
            Subject = null!
        };
    }
}