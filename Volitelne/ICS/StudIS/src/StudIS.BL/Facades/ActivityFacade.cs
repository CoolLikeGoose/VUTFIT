using Microsoft.EntityFrameworkCore;
using StudIS.BL.Facades.Interfaces;
using StudIS.BL.Mappers;
using StudIS.BL.Models;
using StudIS.DAL.Entities;
using StudIS.DAL.Mappers;
using StudIS.DAL.UnitOfWork;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace StudIS.BL.Facades
{
    public class ActivityFacade(IUnitOfWorkFactory unitOfWorkFactory, ActivityModelMapper activityModelMapper)
        : FacadeBase<ActivityEntity, ActivityListModel, ActivityDetailModel, ActivityEntityMapper>
            (unitOfWorkFactory, activityModelMapper), IActivityFacade
    {
        public async Task<IEnumerable<ActivityListModel>> GetByName(string activityName)
        {
            await using IUnitOfWork unitOfWork = UnitOfWorkFactory.Create();
            List<ActivityEntity> entities = await unitOfWork
                .GetRepository<ActivityEntity, ActivityEntityMapper>()
                .Get()
                .Where(e => e.Description.Contains(activityName))
                .ToListAsync()
                .ConfigureAwait(false);

            return ModelMapper.MapToListModel(entities);
        }
        
        

        public async Task<IEnumerable<ActivityListModel>> FilterAsync(string searchTerm, DateTime startDate, DateTime endDate, string selectedActivityType, string subject)
        {
            await using IUnitOfWork unitOfWork = UnitOfWorkFactory.Create();
            var query = unitOfWork
                .GetRepository<ActivityEntity, ActivityEntityMapper>()
                .Get()
                .AsQueryable();

            if (!string.IsNullOrWhiteSpace(searchTerm))
            {
                query = query.Where(e => e.Description.Contains(searchTerm));
            }

            if (startDate != default)
            {
                query = query.Where(e => e.StartTime >= startDate);
            }

            if (endDate != default)
            {
                query = query.Where(e => e.EndTime <= endDate);
            }

            if (!string.IsNullOrWhiteSpace(selectedActivityType) && selectedActivityType != "All")
            {
                query = query.Where(e => e.ActivityType.ToString() == selectedActivityType);
            }

            //TODO: закоментил немного
            // if (!string.IsNullOrWhiteSpace(subject))
            // {
            //     query = query.Where(e => e.Subject.Contains(subject));
            // }

            List<ActivityEntity> entities = await query.ToListAsync().ConfigureAwait(false);
            return ModelMapper.MapToListModel(entities);
        }
    }
}
