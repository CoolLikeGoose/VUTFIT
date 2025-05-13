using StudIS.APP.ViewModels;
using StudIS.APP.ViewModels.Student;
using StudIS.APP.Views;
using StudIS.APP.Views.Activities;
using StudIS.APP.Views.Student;
using StudIS.APP.Views.Subjects;

namespace StudIS.APP;

public static class AppInstaller
{
    public static IServiceCollection AddAppServices(this IServiceCollection services)
    {
        services.Scan(s => s
            .FromAssemblyOf<App>()
            .AddClasses(c => c.AssignableTo<ContentPageBase>())
            .AsSelf()
            .WithTransientLifetime());

        services.Scan(s => s
            .FromAssemblyOf<App>()
            .AddClasses(c => c.AssignableTo<IViewModel>())
            .AsSelfWithInterfaces()
            .WithTransientLifetime()); 
        
        return services;
    }
}