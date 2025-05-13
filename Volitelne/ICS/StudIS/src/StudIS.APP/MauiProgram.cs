using Microsoft.Extensions.Logging;
using StudIS.APP.ViewModels.Student;
using StudIS.APP.ViewModels.Activity;
using StudIS.APP.ViewModels.Subjects;
using StudIS.APP.Views.Activities;
using StudIS.APP.Views.Evaluations;
using StudIS.APP.Views.Student;
using StudIS.APP.Views.Subjects;
using StudIS.BL;
using StudIS.DAL;
using StudIS.DAL.Migrator;
using StudIS.DAL.Options;

namespace StudIS.APP;

public static class MauiProgram
{
    public static MauiApp CreateMauiApp()
    {
        var builder = MauiApp.CreateBuilder();
        builder
            .UseMauiApp<App>()
            .ConfigureFonts(fonts =>
            {
                fonts.AddFont("OpenSans-Regular.ttf", "OpenSansRegular");
                fonts.AddFont("OpenSans-Semibold.ttf", "OpenSansSemibold");
            });

#if DEBUG
        builder.Logging.AddDebug();
#endif

        // Services
        builder.Services
            .AddDalServices(new DALOptions
            {
                DatabaseDirectory = FileSystem.AppDataDirectory,
                DatabaseName = "StudIS.db",
                RecreateDatabaseOnStartup = false,
                SeedDemoData = false
            })
            .AddAppServices()
            .AddBlServices();

        var app = builder.Build();

        // Migration
        app.Services.GetRequiredService<IDbMigrator>().Migrate();

        // Routing
        Routing.RegisterRoute("//students/detail", typeof(StudentDetailView));
        Routing.RegisterRoute("//students/edit", typeof(StudentEditView));
        Routing.RegisterRoute("//activities/detail", typeof(ActivityDetailView));
        Routing.RegisterRoute("//activities/edit", typeof(ActivityEditView));  
        Routing.RegisterRoute("//subjects/detail", typeof(SubjectsDetailView));
        Routing.RegisterRoute("//subjects/edit", typeof(SubjectsEditView));
        Routing.RegisterRoute("//evaluations/edit", typeof(EvaluationEditView));

        return app;
    }
}