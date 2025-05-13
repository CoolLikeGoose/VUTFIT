using Microsoft.EntityFrameworkCore;
using StudIS.BL.Facades.Interfaces;
using StudIS.BL.Mappers;
using StudIS.BL.Models;
using StudIS.DAL.Entities;
using StudIS.DAL.Mappers;
using StudIS.DAL.Repositories;
using StudIS.DAL.UnitOfWork;

namespace StudIS.BL.Facades;

public class StudentFacade(IUnitOfWorkFactory unitOfWorkFactory, StudentModelMapper evaluationModelMapper) 
    : FacadeBase<StudentEntity, StudentListModel, StudentDetailModel, StudentEntityMapper>
        (unitOfWorkFactory, evaluationModelMapper), IStudentFacade
{
    public async Task<IEnumerable<StudentListModel>> GetByName(string studentName)
    {
        await using IUnitOfWork unitOfWork = UnitOfWorkFactory.Create();
        List<StudentEntity> entities = await unitOfWork
            .GetRepository<StudentEntity, StudentEntityMapper>()
            .Get()
            .Where(e => (e.FirstName + " " + e.LastName).ToLower().Contains(studentName) ||
                                    (e.LastName + " " + e.FirstName).ToLower().Contains(studentName))
            .ToListAsync()
            .ConfigureAwait(false);

        return ModelMapper.MapToListModel(entities);
    }

    public override async Task<StudentDetailModel?> GetAsync(Guid id)
    {
        await using IUnitOfWork unitOfWork = UnitOfWorkFactory.Create();
    
        IQueryable<StudentEntity> query = unitOfWork.GetRepository<StudentEntity, StudentEntityMapper>().Get();
        query = query.Include($"{nameof(StudentEntity.Subjects)}.{nameof(StudentsToSubjectsEntity.Subject)}");
        StudentEntity? entity = await query
            .SingleOrDefaultAsync(e => e.Id == id)
            .ConfigureAwait(false);
    
        return entity is null ? null : ModelMapper.MapToDetailModel(entity);
    }

    public async Task<StudentSubjectsDetailModel> SaveSubjectStudentLink(StudentSubjectsListModel model)
    {
        StudentSubjectsDetailModel result;

        StudentSubjectsModelMapper modelMapper = new StudentSubjectsModelMapper();
        await using IUnitOfWork unitOfWork = unitOfWorkFactory.Create();
    
        StudentsToSubjectsEntity entity = modelMapper.MapToEntity(model);
        IRepository<StudentsToSubjectsEntity> repository = unitOfWork.GetRepository<StudentsToSubjectsEntity, StudentsToSubjectsMapper>();

        StudentsToSubjectsEntity insEntity = repository.Insert(entity);
        result = modelMapper.MapToDetailModel(insEntity);

        await unitOfWork.CommitAsync().ConfigureAwait(false);
    
        return result;
    }

    public async Task DeleteSubjectStudentLink(Guid subjectId, Guid studentId)
    {
        await using IUnitOfWork unitOfWork = UnitOfWorkFactory.Create();
        try
        {
            StudentsToSubjectsEntity? item = await unitOfWork
                .GetRepository<StudentsToSubjectsEntity, StudentsToSubjectsMapper>()
                .Get()
                .FirstOrDefaultAsync(e => e.StudentId == studentId && e.SubjectId == subjectId)
                .ConfigureAwait(false);

            if (item != null)
            {
                await unitOfWork.GetRepository<StudentsToSubjectsEntity, StudentsToSubjectsMapper>().DeleteAsync(item.Id).ConfigureAwait(false);
            }
            await unitOfWork.CommitAsync().ConfigureAwait(false);
        }
        catch (DbUpdateException e)
        {
            throw new InvalidOperationException("Cannot delete entity", e);
        }
    }
}