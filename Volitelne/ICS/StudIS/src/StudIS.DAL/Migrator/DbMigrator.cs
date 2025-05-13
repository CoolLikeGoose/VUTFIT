using System.Threading;
using System.Threading.Tasks;
using Microsoft.EntityFrameworkCore;
using StudIS.DAL.Options;

namespace StudIS.DAL.Migrator;

public class DbMigrator(IDbContextFactory<StudIsDbContext> dbContextFactory, DALOptions options) : IDbMigrator
{
    public void Migrate() => MigrateAsync(CancellationToken.None).GetAwaiter().GetResult();
    public async Task MigrateAsync(CancellationToken cancellationToken)
    {
        await using StudIsDbContext dbContext = await dbContextFactory.CreateDbContextAsync(cancellationToken);

        if (options.RecreateDatabaseOnStartup)
        {
            await dbContext.Database.EnsureDeletedAsync(cancellationToken);
        }
            
        await dbContext.Database.EnsureCreatedAsync(cancellationToken);
    }
}