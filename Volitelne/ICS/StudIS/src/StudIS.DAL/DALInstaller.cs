using System;
using Microsoft.EntityFrameworkCore;
using Microsoft.Extensions.DependencyInjection;
using StudIS.DAL.Factories;
using StudIS.DAL.Mappers;
using StudIS.DAL.Migrator;
using StudIS.DAL.Options;

namespace StudIS.DAL;

public static class DalInstaller
{
    public static IServiceCollection AddDalServices(this IServiceCollection services, DALOptions options)
    {
        services.AddSingleton(options);

        if (options is null)
        {
            throw new InvalidOperationException("No DALOptions provided");
        }
        
        if (string.IsNullOrEmpty(options.DatabaseDirectory))
        {
            throw new InvalidOperationException($"{nameof(options.DatabaseDirectory)} is not set");
        }
        if (string.IsNullOrEmpty(options.DatabaseName))
        {
            throw new InvalidOperationException($"{nameof(options.DatabaseName)} is not set");
        }

        services.AddSingleton<IDbContextFactory<StudIsDbContext>>(_ =>
            new DbContextSqLiteFactory(options.DatabaseFilePath, options?.SeedDemoData ?? false));
        services.AddSingleton<IDbMigrator, DbMigrator>();

        services.AddSingleton<ActivityEntityMapper>();
        services.AddSingleton<EvaluationEntityMapper>();
        services.AddSingleton<StudentEntityMapper>();
        services.AddSingleton<SubjectEntityMapper>();
        services.AddSingleton<StudentsToSubjectsMapper>();

        return services;
    }
}