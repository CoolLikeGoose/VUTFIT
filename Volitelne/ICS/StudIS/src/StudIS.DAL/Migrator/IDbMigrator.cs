using System.Threading;
using System.Threading.Tasks;

namespace StudIS.DAL.Migrator;

public interface IDbMigrator
{
    public void Migrate();
    public Task MigrateAsync(CancellationToken cancellationToken);
}