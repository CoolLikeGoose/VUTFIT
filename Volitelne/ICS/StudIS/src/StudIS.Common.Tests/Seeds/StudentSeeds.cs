using Microsoft.EntityFrameworkCore;
using StudIS.DAL.Entities;

namespace StudIS.Common.Tests.Seeds;
public static class StudentSeeds
{
    
    public static readonly StudentEntity StandardInDbStudent1 = new StudentEntity()
    {
        Id = Guid.Parse("51423439-9a23-4d78-aad7-1071b02bb5c4"),
        FirstName = "Nikola",
        LastName = "Testova",
        ImageUrl = "https://examplepicture.example"
    };
    
    public static readonly StudentEntity StandardInDbStudent2 = new StudentEntity()
    {
        Id = Guid.Parse("ae4a7063-1a92-4d5f-8166-70a326f8708b"),
        FirstName = "NeNikola",
        LastName = "NeTestova",
        ImageUrl = "https://api.hub.jhu.edu/factory/sites/default/files/styles/soft_crop_1300/public/depression-hub.jpg"
    };
    
    public static readonly StudentEntity StandardInDbStudent3 = new StudentEntity()
    {
        Id = Guid.Parse("32273ec1-995a-427d-98cd-1b1bc8bb8eca"),
        FirstName = "alokiN",
        LastName = "avotseT",
        ImageUrl = "https://domf5oio6qrcr.cloudfront.net/medialibrary/7813/a83db567-4c93-4ad0-af6f-72b57af7675d.jpg"
    };
    
    public static void Seed(this ModelBuilder modelBuilder)
    {
        modelBuilder.Entity<StudentEntity>().HasData(
            StandardInDbStudent1 with { Subjects = null! },
            StandardInDbStudent2 with { Subjects = null! },
            StandardInDbStudent3  with { Subjects = null! }
        );
    }
}