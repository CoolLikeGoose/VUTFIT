using Xunit.Abstractions;
using Xunit;
using Microsoft.EntityFrameworkCore;
using StudIS.Common.Tests;
using StudIS.Common.Tests.Factories;

namespace StudIS.DAL.Tests;

public class DbContextTestsBase : IAsyncLifetime
{
    protected IDbContextFactory<StudIsDbContext> DbContextFactory { get; }
    protected StudIsDbContext StudIsDbContextSUT { get; }
    
    protected DbContextTestsBase(ITestOutputHelper output)
    {
        XUnitTestOutputConverter converter = new(output);
        Console.SetOut(converter);

        DbContextFactory = new DbContextSqLiteTestingFactory(GetType().FullName!, seedTestingData: true);
        StudIsDbContextSUT = DbContextFactory.CreateDbContext();
    }

    public async Task InitializeAsync()
    {
        await StudIsDbContextSUT.Database.EnsureDeletedAsync();
        await StudIsDbContextSUT.Database.EnsureCreatedAsync();
    }

    public async Task DisposeAsync()
    {
        await StudIsDbContextSUT.Database.EnsureDeletedAsync();
        await StudIsDbContextSUT.DisposeAsync();
    }
}