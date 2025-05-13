using Microsoft.Extensions.DependencyInjection;
using StudIS.BL.Facades;
using StudIS.BL.Facades.Interfaces;
using StudIS.BL.Mappers;
using StudIS.BL.Models;
using StudIS.DAL.Entities;
using StudIS.DAL.Mappers;
using StudIS.DAL.UnitOfWork;

namespace StudIS.BL;

public static class BlInstaller
{
    public static IServiceCollection AddBlServices(this IServiceCollection services)
    {
        services.AddSingleton<IUnitOfWorkFactory, UnitOfWorkFactory>();

        services.Scan(s => s
            .FromAssemblyOf<BusinessLogic>()
            .AddClasses(c => c.AssignableTo(typeof(IFacade<,,>)))
            .AsMatchingInterface()
            .WithSingletonLifetime());

        services.Scan(s => s
            .FromAssemblyOf<BusinessLogic>()
            .AddClasses(c => c.AssignableTo(typeof(IModelMapper<,,>)))
            .AsSelfWithInterfaces()
            .WithSingletonLifetime());

        return services;
    }
}