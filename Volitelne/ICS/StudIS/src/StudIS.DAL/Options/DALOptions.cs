using System.IO;

namespace StudIS.DAL.Options;

public record DALOptions
{
    public required string DatabaseDirectory { get; init; }

    public string DatabaseName { get; init; } = null!;

    public string DatabaseFilePath => Path.Combine(DatabaseDirectory, DatabaseName);

    public bool RecreateDatabaseOnStartup { get; init; } = false;

    public bool SeedDemoData { get; init; } = false;
};