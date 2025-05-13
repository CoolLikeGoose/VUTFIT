using StudIS.BL.Models;
using StudIS.DAL.Entities;
using System;
using System.Collections.Generic;
using System.Threading.Tasks;

namespace StudIS.BL.Facades.Interfaces
{
    public interface IActivityFacade : IFacade<ActivityEntity, ActivityListModel, ActivityDetailModel>
    {
        Task<IEnumerable<ActivityListModel>> GetByName(string searchTerm);
        Task<IEnumerable<ActivityListModel>> FilterAsync(string searchTerm, DateTime startDate, DateTime endDate, string selectedActivityType, string subject);
    }
}